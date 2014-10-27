/*
 * NRF24L01_attiny85.cpp
 *
 * Created: 27/10/2014 16:48:57
 *  Author: arduino
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "Nordic_85.h"

uint8_t Myaddress[] = "att85"; 
uint8_t remoteAddress[] = "_ard_";
uint8_t CH = 10;

int main(void)
{
	Nordic radio(0,1,2);
	radio.cePin = 3;
	radio.csnPin = 4;
	radio.init();
	radio.setRADDR((uint8_t *)Myaddress);
	radio.setTADDR((uint8_t *)remoteAddress);
	radio.payload = 32;
	radio.channel = CH;
	radio.config();
    
	uint8_t aux;
	while(1)
    {
        radio.send(&aux);
		aux++; 
		_delay_ms(1000);
    }
}