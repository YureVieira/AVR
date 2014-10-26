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
	uint8_t pinSS;
	
	public:
	void config(uint8_t miso,uint8_t mosi,uint8_t sclk,uint8_t ss);
	uint8_t shift(uint8_t data);
};

int main(void)
{
	SPI device;
	device.config(0,1,2,3);
	DDRB |= 1<<4;
    while(1)
    {
		device.shift(0xAF);
		_delay_ms(1000);
		PORTB ^= 1<<4;
        //TODO:: Please write your application code 
    }
}
//class SPI
//{
	//private:
	//uint8_t pinMISO;
	//uint8_t pinMOSI;
	//uint8_t pinSCLK;
	//uint8_t pinSS;
	//
	//public:
	//void config(uint8_t miso,uint8_t mosi,uint8_t sclk,uint8_t ss);
	//uint8_t shift(uint8_t data);
//};
/***********************************************************************************************************************************************/
void SPI::config(uint8_t miso,uint8_t mosi,uint8_t sclk,uint8_t ss){
	
	pinMISO = miso;
	pinMOSI = mosi;
	pinSCLK = sclk;
	pinSS = ss;
	DDRB |= (1<<pinMOSI)|(1<<pinSCLK)|(1<<pinSS);
	DDRB &= ~(1<<pinMISO);
}
/***********************************************************************************************************************************************/
uint8_t SPI::shift(uint8_t data){
	uint8_t buffer;
	//buffer = data;
	buffer = data;	
	PORTB |= 1<<pinSS;
	for(uint8_t i;i<8;i++)
	{		
		PORTB |= 1<<pinSCLK;//Coloca a linha de clock em nivel alto.	
		(0x80 & buffer>0) ? PORTB |= 1<<pinMOSI : PORTB &= ~(1<<pinMOSI); //Coloque o bit MSB no pino MOSI.
		buffer<<1;
		(PINB & (1<<pinMISO)>0) ? buffer |= 1<<pinMISO : buffer &= ~(1<<pinMISO); //Coloque o bit contido em pinMISO no buffer.
		_delay_ms(1000);
		PORTB &=~(1<<pinSCLK);//Coloca a linha de clock em nivel baixo.
		_delay_ms(1000);		
	}
	return buffer;
}
/***********************************************************************************************************************************************/