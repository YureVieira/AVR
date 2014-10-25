// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * - File              : LCD.h
 * - Compiler          : IAR EWAVR 4.11A
 *
 * - Support mail      : avr@atmel.com
 *
 * - Supported devices : This example is written for the ATtiny24.
 *
 * - AppNote           : AVR245: Code Lock with LCD.
 *
 * - Description       : Header file for LCD driver.
 *
 * $Revision: 1.3 $
 * $Date: Monday, October 10, 2005 13:23:34 UTC $
 *
 *****************************************************************************/


unsigned char LCD_Init(void);
unsigned char LCD_ClearDisplay(void);
unsigned char LCD_ClearLine(unsigned char row);
unsigned char LCD_PutXY(unsigned char ch, unsigned char column, unsigned char row);
unsigned char LCD_PutXY_Raw(unsigned char ch, unsigned char column, unsigned char row);
unsigned char LCD_WriteLine(unsigned char *msg, unsigned char row);
unsigned char ASCIItoLCD(unsigned char ch);

#define LCD_ADR   0x76
