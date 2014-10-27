#include <avr/io.h>
#include <util/delay.h>
#include "Nordic_85.h"
/***********************************************************************************************************************************************/
uint8_t Nordic::SPIshift(uint8_t data){
	uint8_t buffer;
	buffer = data;	
	for(uint8_t i=0;i<=8;i++)
	{		
		PORTB |= 1<<pinSCLK;//Coloca a linha de clock em nivel alto.	
		(0x80 & buffer) ? PORTB |= 1<<pinMOSI : PORTB &= ~(1<<pinMOSI); //Coloque o bit MSB no pino MOSI.
		buffer = buffer	<<1;
		(PINB & (1<<pinMISO)) ? buffer |= 1<<pinMISO : buffer &= ~(1<<pinMISO); //Coloque o bit contido em pinMISO no buffer.
		_delay_ms(5);
		PORTB &=~(1<<pinSCLK);//Coloca a linha de clock em nivel baixo.
		_delay_ms(5);		
	}
	
	return buffer;
}
/***********************************************************************************************************************************************/
Nordic::Nordic(uint8_t miso,uint8_t mosi,uint8_t sclk){
	pinMISO = miso;
	pinMOSI = mosi;
	pinSCLK = sclk;
	DDRB |= (1<<pinMOSI)|(1<<pinSCLK);
	DDRB &= ~(1<<pinMISO);
	channel = 1;
	payload = 16;
}

void Nordic::transferSync(uint8_t *dataout,uint8_t *datain,uint8_t len){
	uint8_t i;
	for(i = 0;i < len;i++){
		datain[i] = SPIshift(dataout[i]);
	}
}

void Nordic::transmitSync(uint8_t *dataout,uint8_t len){
	uint8_t i;
	for(i = 0;i < len;i++){
		SPIshift(dataout[i]);
	}
}


void Nordic::init() 
// Initializes pins to communicate with the MiRF module
// Should be called in the early initializing phase at startup.
{   
    DDRB |= 1<<cePin;
    DDRB |= 1<<csnPin;
    
    ceLow();
    csnHi();

}


void Nordic::config() 
// Sets the important registers in the MiRF module and powers the module
// in receiving mode
// NB: channel and payload must be set now.
{
    // Set RF channel
	configRegister(RF_CH,channel);

    // Set length of incoming payload 
	configRegister(RX_PW_P0, payload);
	configRegister(RX_PW_P1, payload);

    // Start receiver 
    powerUpRx();
    flushRx();
}

void Nordic::setRADDR(uint8_t * adr) 
// Sets the receiving address
{
	ceLow();
	writeRegister(RX_ADDR_P1,adr,mirf_ADDR_LEN);
	ceHi();
}

void Nordic::setTADDR(uint8_t * adr)
// Sets the transmitting address
{
	/*
	 * RX_ADDR_P0 must be set to the sending addr for auto ack to work.
	 */

	writeRegister(RX_ADDR_P0,adr,mirf_ADDR_LEN);
	writeRegister(TX_ADDR,adr,mirf_ADDR_LEN);
}

bool Nordic::dataReady() 
// Checks if data is available for reading
{
    // See note in getData() function - just checking RX_DR isn't good enough
	uint8_t status = getStatus();

    // We can short circuit on RX_DR, but if it's not set, we still need
    // to check the FIFO for any pending packets
    if ( status & (1 << RX_DR) ) return 1;
    return !rxFifoEmpty();
}

bool Nordic::rxFifoEmpty(){
	uint8_t fifoStatus;

	readRegister(FIFO_STATUS,&fifoStatus,sizeof(fifoStatus));
	return (fifoStatus & (1 << RX_EMPTY));
}



void Nordic::getData(uint8_t * data) 
// Reads payload bytes into data array
{
    csnLow();                               // Pull down chip select
    SPIshift( R_RX_PAYLOAD );            // Send cmd to read rx payload
    transferSync(data,data,payload); // Read payload
    csnHi();                               // Pull up chip select
    // NVI: per product spec, p 67, note c:
    //  "The RX_DR IRQ is asserted by a new packet arrival event. The procedure
    //  for handling this interrupt should be: 1) read payload through SPI,
    //  2) clear RX_DR IRQ, 3) read FIFO_STATUS to check if there are more 
    //  payloads available in RX FIFO, 4) if there are more data in RX FIFO,
    //  repeat from step 1)."
    // So if we're going to clear RX_DR here, we need to check the RX FIFO
    // in the dataReady() function
    configRegister(STATUS,(1<<RX_DR));   // Reset status register
}

void Nordic::configRegister(uint8_t reg, uint8_t value)
// Clocks only one byte into the given MiRF register
{
    csnLow();
    SPIshift(W_REGISTER | (REGISTER_MASK & reg));
    SPIshift(value);
    csnHi();
}

void Nordic::readRegister(uint8_t reg, uint8_t * value, uint8_t len)
// Reads an array of bytes from the given start position in the MiRF registers.
{
    csnLow();
    SPIshift(R_REGISTER | (REGISTER_MASK & reg));
    transferSync(value,value,len);
    csnHi();
}

void Nordic::writeRegister(uint8_t reg, uint8_t * value, uint8_t len) 
// Writes an array of bytes into inte the MiRF registers.
{
    csnLow();
    SPIshift(W_REGISTER | (REGISTER_MASK & reg));
    transmitSync(value,len);
    csnHi();
}


void Nordic::send(uint8_t * value) 
// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
{
    uint8_t status;
    status = getStatus();

    while (PTX) {
	    status = getStatus();

	    if((status & ((1 << TX_DS)  | (1 << MAX_RT)))){
		    PTX = 0;
		    break;
	    }
    }                  // Wait until last paket is send

    ceLow();
    
    powerUpTx();       // Set to transmitter mode , Power up
    
    csnLow();                    // Pull down chip select
    SPIshift( FLUSH_TX );     // Write cmd to flush tx fifo
    csnHi();                    // Pull up chip select
    
    csnLow();                    // Pull down chip select
    SPIshift( W_TX_PAYLOAD ); // Write cmd to write payload
    transmitSync(value,payload);   // Write payload
    csnHi();                    // Pull up chip select

    ceHi();                     // Start transmission
}

/**
 * isSending.
 *
 * Test if chip is still sending.
 * When sending has finished return chip to listening.
 *
 */

bool Nordic::isSending(){
	uint8_t status;
	if(PTX){
		status = getStatus();
	    	
		/*
		 *  if sending successful (TX_DS) or max retries exceded (MAX_RT).
		 */

		if((status & ((1 << TX_DS)  | (1 << MAX_RT)))){
			powerUpRx();
			return false; 
		}

		return true;
	}
	return false;
}

uint8_t Nordic::getStatus(){
	uint8_t rv;
	readRegister(STATUS,&rv,1);
	return rv;
}

void Nordic::powerUpRx(){
	PTX = 0;
	ceLow();
	configRegister(CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (1<<PRIM_RX) ) );
	ceHi();
	configRegister(STATUS,(1 << TX_DS) | (1 << MAX_RT)); 
}

void Nordic::flushRx(){
    csnLow();
    SPIshift( FLUSH_RX );
    csnHi();
}

void Nordic::powerUpTx(){
	PTX = 1;
	configRegister(CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (0<<PRIM_RX) ) );
}

void Nordic::ceHi(){
	PORTB |= 1<<cePin;
}

void Nordic::ceLow(){
	PORTB &= ~(1<<cePin);
}

void Nordic::csnHi(){
	PORTB |= 1<<csnPin;
}

void Nordic::csnLow(){
	PORTB &= ~(1<<csnPin);
}

void Nordic::powerDown(){
	ceLow();
	configRegister(CONFIG, mirf_CONFIG );
}
