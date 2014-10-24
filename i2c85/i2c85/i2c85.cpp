/*
* i2c85.cpp
*
* Created: 18/03/2014 16:27:49
*  Author: Yure
*/

#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "USI_TWI_Master.h"
//#include "usiTwiSlave.h"
#include "i2c_USI.h"

int main(void)
{
	i2c master;
	master.begin();
	uint8_t num=0;
	while(1)
	{		
		master.write(0x0A,num);
		_delay_ms(1000);
		num++;
		//TODO:: Please write your application code
	}
}

