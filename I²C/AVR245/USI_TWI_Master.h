// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * - File              : USI_TWI_Master.h
 * - Compiler          : IAR EWAVR 4.11A
 *
 * - Support mail      : avr@atmel.com
 *
 * - Supported devices : This example is written for the ATtiny24.
 *
 * - AppNote           : AVR245: Code Lock with LCD.
 * - Related AppNote   : AVR310 - Using the USI module as a TWI Master
 *
 * - Description       : This is an implementation of an TWI master using the USI
 *                       module as basis. The implementation assumes the AVR to
 *                       be the only TWI master in the system and can therefore
 *                       not be used in a multi-master system.
 * - Usage             : Initialize the USI module by calling the
 *                       USI_TWI_Master_Initialise() function. Hence messages/data
 *                       are transceived on the bus using the
 *                       USI_TWI_Start_Transceiver_With_Data() function. If the
 *                       transceiver returns with a fail, then use
 *                       USI_TWI_Get_State_Info() to evaluate the couse of failure.
 *
 * $Revision: 1.3 $
 * $Date: Monday, October 10, 2005 13:23:34 UTC $
 *
 *****************************************************************************/


#include <iotiny24.h>   // standard IAR header file with 4.11A and later

//********** Defines **********//

// Defines controlling timing limits
//#define TWI_FAST_MODE

#define SYS_CLK   1000.0  // [kHz]

#ifdef TWI_FAST_MODE               // TWI FAST mode timing limits. SCL = 100-400kHz
  #define T2_TWI    ((SYS_CLK *1300) /1000000) +1 // >1,3us
  #define T4_TWI    ((SYS_CLK * 600) /1000000) +1 // >0,6us

#else                              // TWI STANDARD mode timing limits. SCL <= 100kHz
  #define T2_TWI    ((SYS_CLK *4700) /1000000) +1 // >4,7us
  #define T4_TWI    ((SYS_CLK *4000) /1000000) +1 // >4,0us
#endif

// Defines controling code generating
//#define PARAM_VERIFICATION
//#define NOISE_TESTING
//#define SIGNAL_VERIFY

//USI_TWI messages and flags and bit masks
//#define SUCCESS   7
//#define MSG       0
/****************************************************************************
  Bit and byte definitions
****************************************************************************/

#define W 				0					//Data transfer direction WRITE
#define R 				1					//Data transfer direction READ
#define OWN_ADR 		60  				//Own slave address
#define SUCCESS 		0xFF

#define TWI_READ_BIT  0       // Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1       // Bit position for LSB of the slave address bits in the init byte.
#define TWI_NACK_BIT  0       // Bit position for (N)ACK bit.

#define USI_TWI_NO_DATA             0x00  // Transmission buffer is empty
#define USI_TWI_DATA_OUT_OF_BOUND   0x01  // Transmission buffer is outside SRAM space
#define USI_TWI_UE_START_CON        0x02  // Unexpected Start Condition
#define USI_TWI_UE_STOP_CON         0x03  // Unexpected Stop Condition
#define USI_TWI_UE_DATA_COL         0x04  // Unexpected Data Collision (arbitration)
#define USI_TWI_NO_ACK_ON_DATA      0x05  // The slave did not acknowledge  all data
#define USI_TWI_NO_ACK_ON_ADDRESS   0x06  // The slave did not acknowledge  the address
#define USI_TWI_MISSING_START_CON   0x07  // Generated Start Condition not detected on bus
#define USI_TWI_MISSING_STOP_CON    0x08  // Generated Stop Condition not detected on bus

// Device dependant defines: these are ATtin24 specific
#define DDR_USI             DDRA
#define PORT_USI            PORTA
#define PIN_USI             PINA
#define PORT_USI_SDA        PORTA6
#define PORT_USI_SCL        PORTA4
#define PIN_USI_SDA         PINA6
#define PIN_USI_SCL         PINA4

#define TRUE  1
#define FALSE 0

/****************************************************************************
	Struct definitions
	tx_type is the data struct used as input to the "Send_to_TWI" function
	and contain information about:
	Slave address
	Send or receive data to/from the slave
	Number of bytes to send or receive
	Pointer to a buffer containing the bytes to send, or to a buffer for the
	bytes to receive
****************************************************************************/
typedef struct
{
  unsigned char slave_adr;          //Slave address and W/R byte
  unsigned char size;               //Number of bytes to send or receive 	
  unsigned char *data_ptr;          //Pointer to bytes to send or to RX buffer
} tx_type;

//********** Prototypes **********//

void              USI_TWI_Master_Initialise( void );
unsigned char     USI_TWI_Start_Transceiver_With_Data(unsigned char *, unsigned char);
//__x unsigned char USI_TWI_Start_Transceiver_With_Data(unsigned char *, unsigned char);
//unsigned char     USI_TWI_Start_Transceiver_With_Data(tx_type*);
unsigned char     USI_TWI_LCD_Test(void);
unsigned char     USI_TWI_Get_State_Info( void );

#pragma vector = USI_START_vect
__interrupt void USI_START_ISR(void);
