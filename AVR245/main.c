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
 *
 * - Description       : Code lock application, which uses 4x4 keypad matrix
 *                       and I2C LCD.
 *
 * $Revision: 1.3 $
 * $Date: Monday, October 10, 2005 13:23:34 UTC $
 *
 *****************************************************************************/

/*! \mainpage
*
* \section Intro Introduction
* This documents functions, variables, typedefs, enumerations, enumerator and
* defines in the software for application note AVR245. This is a Code lock
* application, which uses a 4x4 keypad matrix, piezoelectric buzzer and I2C LCD..\n\n
*
* \section CI Compilation Info
* This software was written for IAR Embedded Workbench, 4.11A.\n
* To make project:
* Add the .c files to project (main.c, KPD.c, LCD.c and USI_TWI_Master.c) and
* use project settings: device tiny24, enable bit definitions in I/O include files,
* output format: ubrof8 for Debug and intel_extended for Release.\n
* Please also note that iotiny24.h included with EWAVR 4.11A has wrong address
* for TIMSK0; it is 39 not 3A. The timer interrupt names may also change in
* the next version of EWAVR.\n
*
* Add to Release target: Linker - Extra Options - Use the following command
* line options:\n
* -y(CODE) \n
* -Ointel-extended,(XDATA)=$PROJ_DIR$\\Release\\Exe\\$PROJ_FNAME$_eeprom.hex \n
* This generates a separate file for the eeprom data to overcome limitations in
* the intel extended format.\n
*
* Older versions than IAR Embedded Workbench 4.11A doesn't natively support
* ATtiny24.\n\n
*
* \section DI Device Info
* This example is written for the ATtiny24, but can be easily migrated to
* other AVRs.\n
* Device settings: 1 MHz clock (internal RC).\n\n
*
* \section CSZ Code Size:
* 1607 bytes (IAR 4.11A, maximum optimization).\n\n
*/


#include <inavr.h>

#include <iotiny24.h>   // standard IAR header file with 4.11A and later
#include "main.h"
#include "USI_TWI_Master.h"
#include "LCD.h"
#include "KPD.h"

__flash unsigned char STRING_WELCOME[] = {"Tiny24 CODE LOCK"};
__flash unsigned char STRING_OPEN[] = {"   LOCK  OPEN   "};
__flash unsigned char STRING_CLOSE[] = {"  LOCK  CLOSED  "};

__eeprom unsigned char CODE_ACCESS[CODE_ACCESS_LENGTH+1] = {"1234 "};

unsigned char KeyBuffer[CODE_MAX_LENGTH];

// Main program
void main(void)
{
  unsigned char i;

  Initialise();

  Beep(625);                        // 800 Hz tone at 1 MHz clock
  Delay(100);
  LCD_ClearDisplay();
  ClearInputBuffer();
  Beep(0);

  do                                  // main loop
  {
    for (i=0;i<16;i++)
      LCD_PutXY(STRING_WELCOME[i],i,0); // uses less SRAM than LCD_WriteLine

    while (!KPD_KeyPressed);
    asm("wdr");                       // we have input; reset watchdog

    for (i=CODE_MAX_LENGTH;i>0;i--)
      KeyBuffer[i] = KeyBuffer[i-1];  // scroll input buffer
    KeyBuffer[0] = KPD_GetKey();
    Beep(625);                        // 800 Hz tone at 1 MHz clock
    Delay(100);
    Beep(0);

    for (i=0;i<CODE_MAX_LENGTH;i++)
      LCD_PutXY(KeyBuffer[i],11-i,1);

    CheckAccess();
  }
  while(1);
}

// Master init
void Initialise(void)
{
  USI_TWI_Master_Initialise();        // initialise TWI
  LCD_Init();                         // initialise LCD
  KPD_Init();                         // initialise keypad

  DDRA |= (1 << LOCK_PIN);            // set pin as output (lock control)
  PORTA &= ~(1 << LOCK_PIN);          // set pin low (close lock)

  DDRA |= (1 << PINA5);               // set pin A5 as output (buzzer)

  asm("wdr");
  MCUSR = 0;                          // clear all reset flags
  WDTCSR = (1<<WDIE) | (1<<WDE) | (1<<WDP3);  // enable WD, timeout in 4.0 s
}

void ClearInputBuffer(void)
{
  unsigned char i;

  for (i=0;i<CODE_MAX_LENGTH;i++)
    KeyBuffer[i] = 0;
}

void CheckAccess(void)
{
  unsigned char i;

  for (i=0;i<CODE_ACCESS_LENGTH;i++)
    if (CODE_ACCESS[CODE_ACCESS_LENGTH-i-1] != KeyBuffer[i])
      return;

  // Still here? It was a match, then. Sesame, open.
  OpenLock();
  for (i=0;i<16;i++)
    LCD_PutXY(STRING_CLOSE[i],i,0);   // uses less SRAM than LCD_WriteLine
  Delay(1000);
  KPD_Init();                         // initialise keypad: clear pending keys
}

// Activate signal, which opens the lock.
void OpenLock(void)
{
  unsigned char i;

  for (i=0;i<16;i++)
    LCD_PutXY(STRING_OPEN[i],i,0);    // uses less SRAM than LCD_WriteLine

  Beep(1250);                       // 400 Hz tone at 1 MHz clock

  PORTA |= (1 << LOCK_PIN);           // set pin high (open lock)
  DelayBar(3);
  PORTA &= ~(1 << LOCK_PIN);          // set pin low (close lock)

  Beep(0);
}

// Display a progress bar on second row of LCD. Reduce size of bar from right
// to left, one pixel at a time. Maximum time is 20 seconds (set by input).
void DelayBar(unsigned char sec)
{
  unsigned char i,j;
  unsigned char delay;

  delay = (sec << 2);
  delay += (sec << 3);
  delay += (sec / 2);

  for (i=0;i<16;i++)
    LCD_PutXY_Raw(0x13,i,1);
  for (i=0;i<16;i++)
  {
    for (j=0;j<4;j++)
    {
      LCD_PutXY_Raw(0x7E-j,15-i,1);
      Delay(delay);
    }
    LCD_PutXY(' ',15-i,1);
    Delay(delay);
  }
}

// Sound pietzoelectric buzzer using timer/counter 1. Buzzer frequency is
// inversely proportional to tone input value: Freq = fCLK / (2 x tone).
void Beep(unsigned int tone)
{
  if (tone == 0)
  {
    TCCR1A = (1 << COM1B1);               // clear OC1B on compare match
    TCCR1B = (1 << WGM12);                // CTC mode,  no clock
    TCCR1C = (1 << FOC1B);                // force output compare for channel B
  }
  else
  {
    TCNT1 = 0x0000;                       // clear 16-bit counter
    OCR1A = tone;                         // set TOP
    OCR1B = tone >> 1;                    // set compare
    TCCR1A = (1 << COM1B0);               // toggle OC1B on compare match
    TCCR1B = (1 << WGM12) | (1 << CS10);  // CTC mode, 1/CLK
  }
}

// Delay execution for number of milliseconds. Calibrated for 1 MHz clock.
void Delay(unsigned int ms)
{
  unsigned char delay;

  do
  {
    delay = 166;                      // 6 cycles/loop
    while(delay--);
  }
  while (ms--);
}

// Watchdog timeout.
#pragma vector = WDT_vect
__interrupt void WDT_ISR(void)
{
  MCUSR &= ~(1 << WDRF);              // clear WD reset flag
  WDTCSR |= (1<<WDIE);                // set WDIE to avoid hardware reset
  ClearInputBuffer();                 // erase all keystrokes from buffer
  LCD_ClearLine(1);                   // erase all characters on 2nd row
}
