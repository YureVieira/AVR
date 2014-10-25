// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * - File              : main.h
 * - Compiler          : IAR EWAVR 4.11A
 *
 * - Support mail      : avr@atmel.com
 *
 * - Supported devices : This example is written for the ATtiny24.
 *
 * - AppNote           : AVR245: Code Lock with LCD.
 *
 * - Description       : Code lock application, which uses 4x4 keypad matrix
 *                       and I2C LCD.
 *
 * $Revision: 1.3 $
 * $Date: Monday, October 10, 2005 13:23:34 UTC $
 *
 *****************************************************************************/


#define CODE_MAX_LENGTH     8
#define CODE_ACCESS_LENGTH  4
#define CODE_ADMIN_LENGTH   8
#define BUFSIZE             20
#define LOCK_PIN            PINA7
#define LOOPS_PER_MS        143

void main(void);
void DelayBar(unsigned char sec);
void Initialise(void);
void ClearInputBuffer(void);
void CheckAccess(void);
void OpenLock(void);
void Beep(unsigned int tone);
void Delay(unsigned int ms);

#pragma vector = WDT_vect
__interrupt void WDT_ISR(void);

unsigned char messageBuf[BUFSIZE];

extern unsigned char  KPD_KeyPressed;
extern unsigned char  KPD_LastKey;
