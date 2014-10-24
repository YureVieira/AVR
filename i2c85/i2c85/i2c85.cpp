/*
 * i2c85.cpp
 *
 * Created: 18/03/2014 16:27:49
 *  Author: Yure
 */ 


#include <avr/io.h>
#include "USI_TWI_Master.h"
#include "usiTwiSlave.h"

int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}
class i2c
{
	public:
	uint8_t slave_address;
	void begin();
	void requestFrom();
	void beginTransmission();
	void endTransmission();
	void write(uint8_t data);
	void available();
	void read();
	void onReceive();
	void onRequest();
	};
	
	/*********************************************************************/
	void i2c::begin(uint8_t my_address){
		USI_TWI_Master_Initialise();
		sei();
	}
	void i2c::write(uint8_t data){
		USI_TWI_Master_Transfer(data);
	}
