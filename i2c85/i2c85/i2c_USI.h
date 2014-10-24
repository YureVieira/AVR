#ifndef _I2C_USI_H_
#define _I2C_USI_H_

#include "USI_TWI_Master.h"
#include "usiTwiSlave.h"

class i2c
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
#endif  // ifndef _USI_TWI_SLAVE_H_