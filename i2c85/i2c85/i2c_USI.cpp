#include "USI_TWI_Master.h"
#include "usiTwiSlave.h"
#include "i2c_USI"

/*******************************************************************************************************************************************************************
	Inicializa o hardware.
*******************************************************************************************************************************************************************/
void i2c::begin(){
	USI_TWI_Master_Initialise();
	sei();
}
/*******************************************************************************************************************************************************************
	Inicializa o hardware.
*******************************************************************************************************************************************************************/
void i2c::begin(uint8_t my_address){
	USI_TWI_Master_Initialise();
	sei();
}
/*******************************************************************************************************************************************************************
		Escrita simples.
*******************************************************************************************************************************************************************/
uint8_t i2c::write(uint8_t slave_address,uint8_t data){
	messageBuf[0] = (slave_address<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
	messageBuf[1] = data;             // The second byte is used for the data.                       
	buffer = USI_TWI_Start_Transceiver_With_Data( messageBuf, 2);
	return buffer;
}
/*******************************************************************************************************************************************************************
	Requisite um dado.
*******************************************************************************************************************************************************************/
uint8_t i2c::requestFrom(uint8_t slave_address,uint8_t data){
	messageBuf[0] = (slave_address<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
	messageBuf[1] = data;             // The first byte is used for commands.
	buffer = USI_TWI_Start_Transceiver_With_Data( messageBuf, 2);
	return buffer;
}
/*******************************************************************************************************************************************************************
	Requisite um dado.
*******************************************************************************************************************************************************************/
uint8_t i2c::requestFrom(uint8_t slave_address){
	messageBuf[0] = (slave_address<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
	buffer = USI_TWI_Start_Transceiver_With_Data( messageBuf, 1);
	return buffer;
}