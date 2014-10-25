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
 * - Description       : Driver for 4x4 keypad matrix
 *
 * $Revision: 1.3 $
 * $Date: Monday, October 10, 2005 13:23:34 UTC $
 *
 *****************************************************************************/

#include "KPD.h"

unsigned char KPD_KeyPressed;
unsigned char KPD_LastKey;
unsigned char KPD_ScanRow;
unsigned char KPD_KeyDown;

__flash static unsigned char KPD_Array[4][4] = { {'1','4','7','A'},{'2','5','8','0'},{'3','6','9','B'},{'F','E','D','C'}};

void  KPD_Init(void)
{
  TCCR0B = 0;                         // stop timer/counter

  KPD_KeyPressed = 0;
  KPD_LastKey = 0;
  KPD_ScanRow = 0;
  KPD_KeyDown = 0;

  DDRA |= (1 << PINA0);               // set pin A0 as output (X1)
  DDRA |= (1 << PINA1);               // set pin A1 as output (X2)
  DDRA |= (1 << PINA2);               // set pin A2 as output (X3)
  DDRA |= (1 << PINA3);               // set pin A3 as output (X4)
  PORTA |= (1 << PINA0);              // set pin A0 high
  PORTA |= (1 << PINA1);              // set pin A1 high
  PORTA |= (1 << PINA2);              // set pin A2 high
  PORTA |= (1 << PINA3);              // set pin A3 high

  DDRB &= ~(1 << PINB0);              // set pin B0 to input (Y1)
  DDRB &= ~(1 << PINB1);              // set pin B1 to input (Y2)
  DDRB &= ~(1 << PINB2);              // set pin B2 to input (Y3)
  PORTB |= (1 << PINB0);              // enable pull-up
  PORTB |= (1 << PINB1);              // enable pull-up
  PORTB |= (1 << PINB2);              // enable pull-up

#ifdef DEBUGWIRE
#else
  DDRB &= ~(1 << PINB3);              // set pin B3 to input (Y4)
  PORTB |= (1 << PINB3);              // enable pull-up
#endif

  PCMSK1 |= (1 << PCINT8);            // enable pin change int 8
  PCMSK1 |= (1 << PCINT9);            // enable pin change int 9
  PCMSK1 |= (1 << PCINT10);           // enable pin change int 10

#ifdef DEBUGWIRE
#else
  PCMSK1 |= (1 << PCINT11);           // enable pin change int 11
#endif

  GIMSK |= (1 << PCIE1);              // enable pin change ints
  GIFR |= (1 << PCIF1);               // clear flag

  TCNT0 = 0;                          // clear counter
  TIMSK0 = (1 << TOIE0);              // enable overflow interrupt
  TCCR0A = 0;         // normal mode and normal port operation on all t/c pins
  TCCR0B = (1<<CS01)|(1<<CS00); // normal mode, TOP = 0xFF, TOVF on MAX, CLK/8
                                      // => overflow each 2 ms @ 1 MHz
  asm("sei");                         // enable all interrupts
}

unsigned char KPD_GetKey(void)
{
  if (KPD_KeyPressed)
  {
    KPD_KeyPressed = 0;
    return (KPD_LastKey);
  }
  else
    return (0);
}

// executed when SREG:I = 1 and GIMSK:PCIE1 = 1. GIFR:PCIF1 cleared upon entry.
#pragma vector = PCINT1_vect
__interrupt void PCINT1_ISR(void)
{
  unsigned int delay=2857;             // 7 cycles per count, 20 ms @ 1 MHz
  unsigned char column;

  GIMSK &= ~(1 << PCIE1);             // disable pin change int for de-bounce
  do
  {}
  while(delay--);                     // de-bounce: allow inputs to settle
  GIMSK |= (1 << PCIE1);              // enable pin change ints

#ifdef DEBUGWIRE
  column = (~PINB) & 0x07;
#else
  column = (~PINB) & 0x0F;
#endif

  switch (column)
  {
    case  0x00: KPD_KeyDown = 0;
                break;
    case  0x01: KPD_LastKey = KPD_Array[KPD_ScanRow][0];
                if (!KPD_KeyDown)
                  KPD_KeyPressed = 1;               // must release key before press recorded
                KPD_KeyDown = 1;
                break;
    case  0x02: KPD_LastKey = KPD_Array[KPD_ScanRow][1];
                if (!KPD_KeyDown)
                  KPD_KeyPressed = 1;               // must release key before press recorded
                KPD_KeyDown = 1;
                break;
    case  0x04: KPD_LastKey = KPD_Array[KPD_ScanRow][2];
                if (!KPD_KeyDown)
                  KPD_KeyPressed = 1;               // must release key before press recorded
                KPD_KeyDown = 1;
                break;
    case  0x08: KPD_LastKey = KPD_Array[KPD_ScanRow][3];
                if (!KPD_KeyDown)
                  KPD_KeyPressed = 1;               // must release key before press recorded
                KPD_KeyDown = 1;
                break;
    default:    KPD_LastKey = 0;
                KPD_KeyPressed = 0;
  }
}

#pragma vector = TIM0_OVF_vect
__interrupt void TC0_ISR(void)
{
  if (!KPD_KeyDown)
  {
    PORTA |= (1 << KPD_ScanRow);        // set pin high
    if (++KPD_ScanRow > 3)
      KPD_ScanRow = 0;
    PORTA &= ~(1 << KPD_ScanRow);       // set pin low
  }
}
