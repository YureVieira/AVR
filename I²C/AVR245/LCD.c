// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * - File              : main.c
 * - Compiler          : IAR EWAVR 4.11A
 *
 * - Support mail      : avr@atmel.com
 *
 * - Supported devices : This example is written for the ATtiny24.
 *
 * - AppNote           : AVR245: Code Lock with LCD.
 * - Related AppNote   : Based on AVR155.
 *
 * - Description       : LCD driver for Batron BTHQ 21605AV display, using
 *                       integrated Philips PCF2119RU.
 *
 * $Revision: 1.3 $
 * $Date: Monday, October 10, 2005 13:23:34 UTC $
 *
 *****************************************************************************/

#include "LCD.h"
#include "USI_TWI_Master.h"

extern unsigned char messageBuf[];


unsigned char LCD_Init(void)
{	

  messageBuf[0] = LCD_ADR | (FALSE<<TWI_READ_BIT);  // slave address + R/W mode
  messageBuf[1] = 0x00;               // control byte
  messageBuf[2] = 0x34;               // command: 16 characters x 2 lines
  messageBuf[3] = 0x0C;               // command: display on, cursor off
  messageBuf[4] = 0x06;               // command: increment, no shift
  messageBuf[5] = 0x35;               // command: extended instruction set
  messageBuf[6] = 0x04;               // command: left-to-right, top-to-bottom
  messageBuf[7] = 0x10;               // command: TC1=0, TC2=0
  messageBuf[8] = 0x42;               // command: HV stage 3
  messageBuf[9] = 0x9F;               // command: set VLCD, store VA
  messageBuf[10] = 0x34;              // command: normal instruction set
  messageBuf[11] = 0x80;              // command: DDRAM address = 0x00
  messageBuf[12] = 0x02;              // command: return home

  return (USI_TWI_Start_Transceiver_With_Data(messageBuf,13));
}

unsigned char LCD_ClearDisplay(void)
{
  unsigned char statusTWI;

  statusTWI = LCD_ClearLine(0);
  if (statusTWI)
    return (LCD_ClearLine(1));
  else
    return (statusTWI);
}

unsigned char LCD_ClearLine(unsigned char row)
{
  unsigned char i=0, statusTWI;

  do
    statusTWI = LCD_PutXY_Raw(0xA0,i%16,row);
  while ((i++ < 16) & statusTWI);
  return (statusTWI);
}

unsigned char LCD_PutXY(unsigned char ch, unsigned char column, unsigned char row)
{
  return (LCD_PutXY_Raw(ASCIItoLCD(ch),column,row));
}

unsigned char LCD_PutXY_Raw(unsigned char ch, unsigned char column, unsigned char row)
{
  unsigned char statusTWI;

  messageBuf[0] = LCD_ADR | (FALSE<<TWI_READ_BIT);  // slave address + R/W mode
  messageBuf[1] = 0x00;               // control byte
  if (row == 0)
    messageBuf[2] = 0x80 + column;    // set DDRAM address to 0x00 + X offset
  else
    messageBuf[2] = 0xC0 + column;    // set DDRAM address to 0x10 + X offset
  statusTWI = USI_TWI_Start_Transceiver_With_Data(messageBuf,3);
  if (!statusTWI)
    return (statusTWI);

  messageBuf[0] = LCD_ADR | (FALSE<<TWI_READ_BIT);  // slave address + R/W mode
  messageBuf[1] = 0x40;               // control byte for data
  messageBuf[2] = ch;
  return (USI_TWI_Start_Transceiver_With_Data(messageBuf,3));
}

unsigned char LCD_WriteLine(unsigned char *msg, unsigned char row)
{
  unsigned char i, statusTWI;

  messageBuf[0] = LCD_ADR | (FALSE<<TWI_READ_BIT);  // slave address + R/W mode
  messageBuf[1] = 0x00;               // control byte
  if (row == 0)
    messageBuf[2] = 0x80;             // command: set DDRAM address to 0x00
  else
    messageBuf[2] = 0xC0;             // command: set DDRAM address to 0x10
  statusTWI = USI_TWI_Start_Transceiver_With_Data(messageBuf,3);
  if (!statusTWI)
    return (statusTWI);

  messageBuf[0] = LCD_ADR | (FALSE<<TWI_READ_BIT);  // slave address + R/W mode
  messageBuf[1] = 0x40;               // control byte for data
  for (i=0;i<16;i++)
    messageBuf[2+i] = ASCIItoLCD(*(msg++));
  return (USI_TWI_Start_Transceiver_With_Data(messageBuf,18));
}

unsigned char ASCIItoLCD(unsigned char ch)
{
  unsigned char c;

  c = 0x40;
  if ((ch >= 0x20) & (ch <= 0x3F))
    c = 0x80 + ch;
  if ((ch >= 0x41) & (ch <= 0x5A))
    c = 0x80 + ch;
  if ((ch >= 0x61) & (ch <= 0x7A))
    c = 0x80 + ch;

  return c;
}
