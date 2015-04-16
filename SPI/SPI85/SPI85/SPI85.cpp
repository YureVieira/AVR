/*
 * SPI85.cpp
 *
 * Created: 25/10/2014 21:01:57
 *  Author: Yure
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#define pinCS 4

class SPI
{
	private:
	uint8_t pinMISO;
	uint8_t pinMOSI;
	uint8_t pinSCK;
	uint8_t _mode;
	
	public:
	void config(uint8_t miso,uint8_t mosi,uint8_t SCK,uint8_t mode=0);
	uint8_t transfer(uint8_t data);
};

int main(void)
{
	SPI device;
	device.config(0,1,2);
	DDRB |= 1<<pinCS;
	PORTB |= 1<<pinCS; 
	uint8_t aux=0xff;
    while(1)
    {
		PORTB &= ~(1<<pinCS);
		device.transfer(aux);
		PORTB |= (1<<pinCS);	
		
		_delay_ms(100);
		aux--;
		//if(aux==10)aux=0;
        //TODO:: Please write your application code 
    }
}
/***********************************************************************************************************************************************/
void SPI::config(uint8_t miso,uint8_t mosi,uint8_t SCK,uint8_t mode){
	pinMISO = miso;
	pinMOSI = mosi;
	pinSCK = SCK;
	DDRB |= (1<<pinMOSI)|(1<<pinSCK);
	DDRB &= ~(1<<pinMISO);
	
	if(mode == 2 || mode == 3)
	PORTB |= (1<<pinSCK);             //Repouso em alto
	else
	PORTB &= ~(1<<pinSCK);             //Repouso em baixo
}
/***********************************************************************************************************************************************/
uint8_t SPI::transfer(uint8_t data){
uint8_t _byte;
_byte = data;
if(_mode==1 || _mode==3)
{
	for(int i=0;i<8;i++)
	{
		if(_byte & 0x80) PORTB |= (1<<pinMOSI);
		else PORTB &= ~(1<<pinMOSI);
		_delay_us(10);
		PORTB ^= (1<<pinSCK); /* a slave shifts out output data bit */
		_byte <<= 1;
		if((PORTB & (1<<pinMISO))) _byte |= 0x01;
		_delay_us(10);
		PORTB ^= (1<<pinSCK); /* a slave latches input data bit */
	}
}
else
{
	for(int i=0;i<8;i++)
	{
		if(_byte & 0x80)PORTB |= (1<<pinMOSI);
		else PORTB &= ~(1<<pinMOSI);
		_delay_us(10);
		PORTB ^= (1<<pinSCK); /* a slave latches input data bit */
		_byte <<= 1;
		if((PORTB & (1<<pinMISO))) _byte |= 0x01;
		_delay_us(10);
		PORTB ^= (1<<pinSCK);	
	}
}
return _byte;
}
/***********************************************************************************************************************************************/