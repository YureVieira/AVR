// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * - File              : KPD.h
 * - Compiler          : IAR EWAVR 4.11A
 *
 * - Support mail      : avr@atmel.com
 *
 * - Supported devices : This example is written for the ATtiny24.
 *
 * - AppNote           : AVR245: Code Lock with LCD.
 *
 * - Description       : Driver for 4x4 keypad matrix
 *
 * $Revision: 1.3 $
 * $Date: Monday, October 10, 2005 13:23:34 UTC $
 *
 *****************************************************************************/


#include <iotiny24.h>   // standard IAR header file with 4.11A and later

// define this when debugging (debugWIRE uses same pin as one of the keypad
// interface lines)
#define DEBUGWIRE

void  KPD_Init(void);
unsigned char KPD_GetKey(void);

#pragma vector = PCINT1_vect
__interrupt void PCINT1_ISR(void);

#pragma vector = TIM0_OVF_vect
__interrupt void TC0_ISR(void);
