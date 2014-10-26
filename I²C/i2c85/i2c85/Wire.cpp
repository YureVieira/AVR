/*
	Biblioteca usa funções e definições da biblioteca USI_TWI_Master da atmel.
*/
#include "Wire.h"
#include "usiTwiSlave.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//********** Defines **********//

// Defines controlling timing limits
#define TWI_FAST_MODE

#define SYS_CLK   4000.0  // [kHz]

#ifdef TWI_FAST_MODE               // TWI FAST mode timing limits. SCL = 100-400kHz
  #define T2_TWI    ((SYS_CLK *1300) /1000000) +1 // >1,3us
  #define T4_TWI    ((SYS_CLK * 600) /1000000) +1 // >0,6us
  
#else                              // TWI STANDARD mode timing limits. SCL <= 100kHz
  #define T2_TWI    ((SYS_CLK *4700) /1000000) +1 // >4,7us
  #define T4_TWI    ((SYS_CLK *4000) /1000000) +1 // >4,0us
#endif

// Defines controling code generating
//#define PARAM_VERIFICATION
//#define NOISE_TESTING
//#define SIGNAL_VERIFY

//USI_TWI messages and flags and bit masks
//#define SUCCESS   7
//#define MSG       0
/****************************************************************************
  Bit and byte definitions
****************************************************************************/
#define TWI_READ_BIT  0       // Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1       // Bit position for LSB of the slave address bits in the init byte.
#define TWI_NACK_BIT  0       // Bit position for (N)ACK bit.

#define USI_TWI_NO_DATA             0x00  // Transmission buffer is empty
#define USI_TWI_DATA_OUT_OF_BOUND   0x01  // Transmission buffer is outside SRAM space
#define USI_TWI_UE_START_CON        0x02  // Unexpected Start Condition
#define USI_TWI_UE_STOP_CON         0x03  // Unexpected Stop Condition
#define USI_TWI_UE_DATA_COL         0x04  // Unexpected Data Collision (arbitration)
#define USI_TWI_NO_ACK_ON_DATA      0x05  // The slave did not acknowledge  all data
#define USI_TWI_NO_ACK_ON_ADDRESS   0x06  // The slave did not acknowledge  the address
#define USI_TWI_MISSING_START_CON   0x07  // Generated Start Condition not detected on bus
#define USI_TWI_MISSING_STOP_CON    0x08  // Generated Stop Condition not detected on bus

// Device dependant defines

#if defined(__AVR_AT90Mega169__) | defined(__AVR_ATmega169PA__) | \
    defined(__AVR_AT90Mega165__) | defined(__AVR_ATmega165__) | \
    defined(__AVR_ATmega325__) | defined(__AVR_ATmega3250__) | \
    defined(__AVR_ATmega645__) | defined(__AVR_ATmega6450__) | \
    defined(__AVR_ATmega329__) | defined(__AVR_ATmega3290__) | \
    defined(__AVR_ATmega649__) | defined(__AVR_ATmega6490__)
    #define DDR_USI             DDRE
    #define PORT_USI            PORTE
    #define PIN_USI             PINE
    #define PORT_USI_SDA        PORTE5
    #define PORT_USI_SCL        PORTE4
    #define PIN_USI_SDA         PINE5
    #define PIN_USI_SCL         PINE4
#endif

#if defined(__AVR_ATtiny25__) | defined(__AVR_ATtiny45__) | defined(__AVR_ATtiny85__) | \
    defined(__AVR_AT90Tiny26__) | defined(__AVR_ATtiny26__)
    #define DDR_USI             DDRB
    #define PORT_USI            PORTB
    #define PIN_USI             PINB
    #define PORT_USI_SDA        PORTB0
    #define PORT_USI_SCL        PORTB2
    #define PIN_USI_SDA         PINB0
    #define PIN_USI_SCL         PINB2
#endif

#if defined(__AVR_AT90Tiny2313__) | defined(__AVR_ATtiny2313__)
    #define DDR_USI             DDRB
    #define PORT_USI            PORTB
    #define PIN_USI             PINB
    #define PORT_USI_SDA        PORTB5
    #define PORT_USI_SCL        PORTB7
    #define PIN_USI_SDA         PINB5
    #define PIN_USI_SCL         PINB7
#endif

// General defines
#define TRUE  1
#define FALSE 0
/******************************************************************************************************************************************************************/
unsigned char USI_TWI_Master_Transfer( unsigned char );
unsigned char USI_TWI_Master_Stop( void );

union  USI_TWI_state
{
  unsigned char errorState;         // Can reuse the TWI_state for error states due to that it will not be need if there exists an error.
  struct
  {
    unsigned char addressMode         : 1;
    unsigned char masterWriteDataMode : 1;
    unsigned char unused              : 6;
  }; 
}   USI_TWI_state;

/*---------------------------------------------------------------
 USI TWI single master initialization function
---------------------------------------------------------------*/
void USI_TWI_Master_Initialise( void )
{
  PORT_USI |= (1<<PIN_USI_SDA);           // Enable pullup on SDA, to set high as released state.
  PORT_USI |= (1<<PIN_USI_SCL);           // Enable pullup on SCL, to set high as released state.
  
  DDR_USI  |= (1<<PIN_USI_SCL);           // Enable SCL as output.
  DDR_USI  |= (1<<PIN_USI_SDA);           // Enable SDA as output.
  
  USIDR    =  0xFF;                       // Preload dataregister with "released level" data.
  USICR    =  (0<<USISIE)|(0<<USIOIE)|                            // Disable Interrupts.
              (1<<USIWM1)|(0<<USIWM0)|                            // Set USI in Two-wire mode.
              (1<<USICS1)|(0<<USICS0)|(1<<USICLK)|                // Software stobe as counter clock source
              (0<<USITC);
  USISR   =   (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Clear flags,
              (0x0<<USICNT0);                                     // and reset counter.
}

/*---------------------------------------------------------------
Use this function to get hold of the error message from the last transmission
---------------------------------------------------------------*/
unsigned char USI_TWI_Get_State_Info( void )
{
  return ( USI_TWI_state.errorState );                            // Return error state.
}

/*---------------------------------------------------------------
 USI Transmit and receive function. LSB of first byte in data 
 indicates if a read or write cycles is performed. If set a read
 operation is performed.

 Function generates (Repeated) Start Condition, sends address and
 R/W, Reads/Writes Data, and verifies/sends ACK.
 
 Success or error code is returned. Error codes are defined in 
 USI_TWI_Master.h
---------------------------------------------------------------*/
unsigned char USI_TWI_Start_Transceiver_With_Data( unsigned char *msg, unsigned char msgSize)
{
  unsigned char tempUSISR_8bit = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Prepare register value to: Clear flags, and
                                 (0x0<<USICNT0);                                     // set USI to shift 8 bits i.e. count 16 clock edges.
  unsigned char tempUSISR_1bit = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Prepare register value to: Clear flags, and
                                 (0xE<<USICNT0);                                     // set USI to shift 1 bit i.e. count 2 clock edges.

  USI_TWI_state.errorState = 0;
  USI_TWI_state.addressMode = TRUE;

#ifdef PARAM_VERIFICATION
  if(msg > (unsigned char*)RAMEND)                 // Test if address is outside SRAM space
  {
    USI_TWI_state.errorState = USI_TWI_DATA_OUT_OF_BOUND;
    return (FALSE);
  }
  if(msgSize <= 1)                                 // Test if the transmission buffer is empty
  {
    USI_TWI_state.errorState = USI_TWI_NO_DATA;
    return (FALSE);
  }
#endif

#ifdef NOISE_TESTING                                // Test if any unexpected conditions have arrived prior to this execution.
  if( USISR & (1<<USISIF) )
  {
    USI_TWI_state.errorState = USI_TWI_UE_START_CON;
    return (FALSE);
  }
  if( USISR & (1<<USIPF) )
  {
    USI_TWI_state.errorState = USI_TWI_UE_STOP_CON;
    return (FALSE);
  }
  if( USISR & (1<<USIDC) )
  {
    USI_TWI_state.errorState = USI_TWI_UE_DATA_COL;
    return (FALSE);
  }
#endif

  if ( !(*msg & (1<<TWI_READ_BIT)) )                // The LSB in the address byte determines if is a masterRead or masterWrite operation.
  {
    USI_TWI_state.masterWriteDataMode = TRUE;
  }

/* Release SCL to ensure that (repeated) Start can be performed */
  PORT_USI |= (1<<PIN_USI_SCL);                     // Release SCL.
  while( !(PIN_USI & (1<<PIN_USI_SCL)) );          // Verify that SCL becomes high.
#ifdef TWI_FAST_MODE
  _delay_us( T4_TWI/4 );                         // Delay for T4TWI if TWI_FAST_MODE
#else
  _delay_us( T2_TWI/4 );                         // Delay for T2TWI if TWI_STANDARD_MODE
#endif

/* Generate Start Condition */
  PORT_USI &= ~(1<<PIN_USI_SDA);                    // Force SDA LOW.
  _delay_us( T4_TWI/4 );                         
  PORT_USI &= ~(1<<PIN_USI_SCL);                    // Pull SCL LOW.
  PORT_USI |= (1<<PIN_USI_SDA);                     // Release SDA.

#ifdef SIGNAL_VERIFY
  if( !(USISR & (1<<USISIF)) )
  {
    USI_TWI_state.errorState = USI_TWI_MISSING_START_CON;  
    return (FALSE);
  }
#endif

/*Write address and Read/Write data */
  do
  {
    /* If masterWrite cycle (or inital address tranmission)*/
    if (USI_TWI_state.addressMode || USI_TWI_state.masterWriteDataMode)
    {
      /* Write a byte */
      PORT_USI &= ~(1<<PIN_USI_SCL);                // Pull SCL LOW.
      USIDR     = *(msg++);                        // Setup data.
      USI_TWI_Master_Transfer( tempUSISR_8bit );    // Send 8 bits on bus.
      
      /* Clock and verify (N)ACK from slave */
      DDR_USI  &= ~(1<<PIN_USI_SDA);                // Enable SDA as input.
      if( USI_TWI_Master_Transfer( tempUSISR_1bit ) & (1<<TWI_NACK_BIT) ) 
      {
        if ( USI_TWI_state.addressMode )
          USI_TWI_state.errorState = USI_TWI_NO_ACK_ON_ADDRESS;
        else
          USI_TWI_state.errorState = USI_TWI_NO_ACK_ON_DATA;
        return (FALSE);
      }
      USI_TWI_state.addressMode = FALSE;            // Only perform address transmission once.
    }
    /* Else masterRead cycle*/
    else
    {
      /* Read a data byte */
      DDR_USI   &= ~(1<<PIN_USI_SDA);               // Enable SDA as input.
      *(msg++)  = USI_TWI_Master_Transfer( tempUSISR_8bit );

      /* Prepare to generate ACK (or NACK in case of End Of Transmission) */
      if( msgSize == 1)                            // If transmission of last byte was performed.
      {
        USIDR = 0xFF;                              // Load NACK to confirm End Of Transmission.
      }
      else
      {
        USIDR = 0x00;                              // Load ACK. Set data register bit 7 (output for SDA) low.
      }
      USI_TWI_Master_Transfer( tempUSISR_1bit );   // Generate ACK/NACK.
    }
  }
  while( --msgSize) ;                             // Until all data sent/received.
  
  USI_TWI_Master_Stop();                           // Send a STOP condition on the TWI bus.

/* Transmission successfully completed*/
  return (TRUE);
}

/*---------------------------------------------------------------
 Core function for shifting data in and out from the USI.
 Data to be sent has to be placed into the USIDR prior to calling
 this function. Data read, will be return'ed from the function.
---------------------------------------------------------------*/
unsigned char USI_TWI_Master_Transfer( unsigned char temp )
{
  USISR = temp;                                     // Set USISR according to temp.
                                                    // Prepare clocking.
  temp  =  (0<<USISIE)|(0<<USIOIE)|                 // Interrupts disabled
           (1<<USIWM1)|(0<<USIWM0)|                 // Set USI in Two-wire mode.
           (1<<USICS1)|(0<<USICS0)|(1<<USICLK)|     // Software clock strobe as source.
           (1<<USITC);                              // Toggle Clock Port.
  do
  {
    _delay_us( T2_TWI/4 );              
    USICR = temp;                          // Generate positve SCL edge.
    while( !(PIN_USI & (1<<PIN_USI_SCL)) );// Wait for SCL to go high.
    _delay_us( T4_TWI/4 );              
    USICR = temp;                          // Generate negative SCL edge.
  }
  while( !(USISR & (1<<USIOIF)) );        // Check for transfer complete.
  
  _delay_us( T2_TWI/4 );                
  temp  = USIDR;                           // Read out data.
  USIDR = 0xFF;                            // Release SDA.
  DDR_USI |= (1<<PIN_USI_SDA);             // Enable SDA as output.

  return temp;                             // Return the data from the USIDR
}


/*---------------------------------------------------------------
 Function for generating a TWI Stop Condition. Used to release 
 the TWI bus.
---------------------------------------------------------------*/
unsigned char USI_TWI_Master_Stop( void )
{
  PORT_USI &= ~(1<<PIN_USI_SDA);           // Pull SDA low.
  PORT_USI |= (1<<PIN_USI_SCL);            // Release SCL.
  while( !(PIN_USI & (1<<PIN_USI_SCL)) );  // Wait for SCL to go high.
  _delay_us( T4_TWI/4 );               
  PORT_USI |= (1<<PIN_USI_SDA);            // Release SDA.
  _delay_us( T2_TWI/4 );                
  
#ifdef SIGNAL_VERIFY
  if( !(USISR & (1<<USIPF)) )
  {
    USI_TWI_state.errorState = USI_TWI_MISSING_STOP_CON;    
    return (FALSE);
  }
#endif

  return (TRUE);
}


/******************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************
Inicializa o hardware.
*******************************************************************************************************************************************************************/
void wire::begin(){
	USI_TWI_Master_Initialise();
	sei();
}
/*******************************************************************************************************************************************************************
Inicializa o hardware.
*******************************************************************************************************************************************************************/
void wire::begin(uint8_t my_address){
	USI_TWI_Master_Initialise();
	sei();
}
/*******************************************************************************************************************************************************************
Escrita simples.
*******************************************************************************************************************************************************************/
uint8_t wire::write(uint8_t slave_address,uint8_t data){
	messageBuf[0] = (slave_address<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
	messageBuf[1] = data;             // The second byte is used for the data.
	buffer = USI_TWI_Start_Transceiver_With_Data( messageBuf, 2);
	return buffer;
}
/*******************************************************************************************************************************************************************
Requisite um dado.
*******************************************************************************************************************************************************************/
uint8_t wire::requestFrom(uint8_t slave_address,uint8_t data){
	messageBuf[0] = (slave_address<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
	messageBuf[1] = data;             // The first byte is used for commands.
	buffer = USI_TWI_Start_Transceiver_With_Data( messageBuf, 2);
	return buffer;
}
/*******************************************************************************************************************************************************************
Requisite um dado.
*******************************************************************************************************************************************************************/
uint8_t wire::requestFrom(uint8_t slave_address){
	messageBuf[0] = (slave_address<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
	buffer = USI_TWI_Start_Transceiver_With_Data( messageBuf, 1);
	return buffer;
}

