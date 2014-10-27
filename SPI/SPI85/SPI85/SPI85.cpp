/*
 * SPI85.cpp
 *
 * Created: 25/10/2014 21:01:57
 *  Author: Yure
 */ 


#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

class SPI
{
	private:
	uint8_t pinMISO;
	uint8_t pinMOSI;
	uint8_t pinSCLK;
	
	public:
	void config(uint8_t miso,uint8_t mosi,uint8_t sclk);
	uint8_t shift(uint8_t data);
};

int main(void)
{
	SPI device;
	device.config(0,1,2);
	DDRB |= 1<<4;
	uint8_t aux=0;
    while(1)
    {
		device.shift(aux);
		_delay_ms(1000);
		PORTB ^= 1<<4;		
		aux++;
        //TODO:: Please write your application code 
    }
}
/***********************************************************************************************************************************************/
void SPI::config(uint8_t miso,uint8_t mosi,uint8_t sclk){
	pinMISO = miso;
	pinMOSI = mosi;
	pinSCLK = sclk;
	DDRB |= (1<<pinMOSI)|(1<<pinSCLK);
	DDRB &= ~(1<<pinMISO);
}
/***********************************************************************************************************************************************/
uint8_t SPI::shift(uint8_t data){
	uint8_t buffer;
	//buffer = data;
	buffer = data;	
	for(uint8_t i=0;i<=8;i++)
	{		
		PORTB |= 1<<pinSCLK;//Coloca a linha de clock em nivel alto.	
		(0x80 & buffer) ? PORTB |= 1<<pinMOSI : PORTB &= ~(1<<pinMOSI); //Coloque o bit MSB no pino MOSI.
		buffer = buffer	<<1;
		(PINB & (1<<pinMISO)) ? buffer |= 1<<pinMISO : buffer &= ~(1<<pinMISO); //Coloque o bit contido em pinMISO no buffer.
		_delay_ms(100);
		PORTB &=~(1<<pinSCLK);//Coloca a linha de clock em nivel baixo.
		_delay_ms(100);		
	}	
	return buffer;
}
/***********************************************************************************************************************************************/