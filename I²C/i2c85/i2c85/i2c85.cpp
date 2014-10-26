/*
* i2c85.cpp
*
* Created: 18/03/2014 16:27:49
*  Author: Yure
*/

#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "USI_TWI_Master.h"
//#include "usiTwiSlave.h"
#include "Wire.h"

int main(void)
{
	DDRB |= 1<<1;
	wire device; 
	device.begin();
	uint8_t num=0;
	while(1)
	{
		device.write(0x20,num);
		PORTB ^= 1<<1; 
		_delay_ms(1000);
		num++;
		//TODO:: Please write your application code
	}
}
