#ifndef _WIRE_H_
#define _WIRE_H_
#include <avr/io.h>
class wire
{
	private:
	uint8_t buffer;
	
	public:
	uint8_t messageBuf[4];
	
	void begin();
	void begin(uint8_t my_address);
	uint8_t requestFrom(uint8_t slave_address);
	uint8_t requestFrom(uint8_t slave_address,uint8_t data);
	uint8_t write(uint8_t slave_address,uint8_t data);
};

#endif

