/*
* i2c_att85.cpp
*
* Created: 11/04/2014 16:38:46
*  Author: Yure
*/


#include <avr/io.h>

int main(void)
{
	while(1)
	{
		//TODO:: Please write your application code
	}
}
class twi
{
	private:
	uint8_t data;
	public:
	uint8_t address;
	twi();
	void read();
};
twi::twi(uint8_t addr)
{
	address = addr;
	usiTwiSlaveInit(slaveAddress);
	clock_select();
}