/***************************************************************************
 *
 * File Name        : "iotiny24.h"
 * Title            : Register/Bit Definitions for the ATtiny24
 * Date             : 02.08, 2005
 * Version          : 0.1
 * Support telephone: not supported version
 * Support fax      : not supported version
 * Support E-Mail   : not supported version
 * Target MCU       : ATtiny24
 *
 * This file #defines the internal register addresses for ATtiny24.
 * Used with iccAVR and aAVR.
 *
 **************************************************************************/

#include "iomacro.h"

#if TID_GUARD(0)
#error This file should only be compiled with iccavr or aavr with processor option -v0
#endif /* TID_GUARD(0) */

/* Include the SFR part if this file has not been included before,
 * OR this file is included by the assembler (SFRs must be defined in
 * each assembler module). */
#if !defined(__IOTINY24_H) || defined(__IAR_SYSTEMS_ASM__)

#pragma language=extended

 /* =========================*/
 /* Predefined SFR Addresses */
 /* =========================*/

SFR_B_N(0x3F, SREG, I, T, H, S, V, N, Z, C)
SFR_W_N(0x3D, SP, Dummy15, Dummy14, Dummy13, Dummy12, Dummy11, Dummy10, SP9, SP8, SP7, SP6, SP5, SP4, SP3, SP2, SP1, SP0) // edit 30.09.05 RKS
SFR_B_R(0x3C, OCR0B);
SFR_B_N(0x3B, GIMSK, Dummy7, INT0, PCIE1, PCIE0, Dummy3, Dummy2, Dummy1, Dummy0)
SFR_B_N(0x3A, GIFR, Dummy7, INTF0, PCIF1, PCIF0, Dummy3, Dummy2, Dummy1, Dummy0)
SFR_B_N(0x39, TIMSK0, Dummy7, Dummy6, Dummy5, Dummy4, Dummy3, OCIE0B, OCIE0A, TOIE0)
SFR_B_N(0x38, TIFR0, Dummy7, Dummy6, Dummy5, Dummy4, Dummy3, OCF0B, OCF0A, TOV0)
SFR_B_N(0x37, SPMCSR, Dummy7, Dummy6, Dummy5, CTPB, RFLB, PGWRT, PGERS, SPMEN)
SFR_B_R(0x36, OCR0A)
SFR_B_N(0x35, MCUCR, Dummy7, PUD, SE, SM1, SM0, Dummy2, ISC01, ISC00)
SFR_B_N(0x34, MCUSR, Dummy7, Dummy6, Dummy5, Dummy4, WDRF, BORF, EXTRF, PORF)
SFR_B_N(0x33, TCCR0B, FOC0A, FOC0B, Dummy5, Dummy4, WGM02, CS02, CS01, CS00)
SFR_B_R(0x32, TCNT0)
SFR_B_R(0x31, OSCCAL)
SFR_B_N(0x30, TCCR0A, COM0A1, COM0A0, COM0B1, COM0B0, Dummy3, Dummy2, WGM01, WGM00)
SFR_B_N(0x2F, TCCR1A, COM1A1, COM1A0, COM1B1, COM1B0, Dummy3, Dummy2, WGM11, WGM10)
SFR_B_N(0x2E, TCCR1B, ICNC1, ICES1, Dummy5, WGM13, WGM12, CS12, CS11, CS10)
//SFR_W_R(0x2D, TCNT1)
SFR_W_R(0x2C, TCNT1)
//SFR_W_R(0x2B, OCR1A)
SFR_W_R(0x2A, OCR1A)
//SFR_W_R(0x29, OCR1B)
SFR_W_R(0x28, OCR1B)
SFR_B_R(0x27, DWDR)
SFR_B_N(0x26, CLKPR, CLKPCE, Dummy6, Dummy5, Dummy4, CLKPS3, CLKPS2, CLKPS1, CLKPS0)
//SFR_W_R(0x25, ICR1)
SFR_W_R(0x24, ICR1)
SFR_B_N(0x23, GTCCR, TSM, Dummy6, Dummy5, Dummy4, Dummy3, Dummy2, Dummy1, PSR10)
SFR_B_N(0x22, TCCR1C, FOC1A, FOC1B, Dummy5, Dummy4, Dummy3, Dummy2, Dummy1, Dummy0)
SFR_B_N(0x21, WDTCSR, WDIF, WDIE, WDP3, WDCE, WDE, WDP2, WDP1, WDP0)
SFR_B_N(0x20, PCMSK1, Dummy7, Dummy6, Dummy5, Dummy4, PCINT11, PCINT10, PCINT9, PCINT8)
SFR_W_N(0x1F, EEAR, Dummy15, Dummy14, Dummy13, Dummy12, Dummy11, Dummy10, Dummy9, EEAR8, EEAR7, EEAR6, EEAR5, EEAR4, EEAR3, EEAR2, EEAR1, EEAR0)
SFR_B_R(0x1D, EEDR)
SFR_B_N(0x1C, EECR, Dummy7, Dummy6, EEPM1, EEPM0, EERIE, EEMPE, EEPE, EERE)
SFR_B_N(0x1B, PORTA, PORTA7, PORTA6, PORTA5, PORTA4, PORTA3, PORTA2, PORTA1, PORTA0)
SFR_B_N(0x1A, DDRA, DDA7, DDA6, DDA5, DDA4, DDA3, DDA2, DDA1, DDA0)
SFR_B_N(0x19, PINA, PINA7, PINA6, PINA5, PINA4, PINA3, PINA2, PINA1, PINA0)
SFR_B_N(0x18, PORTB, Dummy7, Dummy6, Dummy5, Dummy4, PORTB3, PORTB2, PORTB1, PORTB0)
SFR_B_N(0x17, DDRB, Dummy7, Dummy6, Dummy5, Dummy4, DDB3, DDB2, DDB1, DDB0)
SFR_B_N(0x16, PINB, Dummy7, Dummy6, Dummy5, Dummy4, PINB3, PINB2, PINB1, PINB0)
SFR_B_R(0x15, GPIOR2)
SFR_B_R(0x14, GPIOR1)
SFR_B_R(0x13, GPIOR0)
SFR_B_N(0x12, PCMSK0, PCINT7, PCINT6, PCINT5, PCINT4, PCINT3, PCINT2, PCINT1, PCINT0)
SFR_B_R(0x10, USIBR)
SFR_B_R(0x0F, USIDR)
SFR_B_N(0x0E, USISR, USISIF, USIOIF, USIPF, USIDC, USICNT3, USICNT2, USICNT1, USICNT0)
SFR_B_N(0x0D, USICR, USISIE, USIOIE, USIWM1, USIWM0, USICS1, USICS0, USICLK, USITC)
SFR_B_N(0x0C, TIMSK1, Dummy7, Dummy6, ICIE1, Dummy4, Dummy3, OCIE1B, OCIE1A, TOIE1)
SFR_B_N(0x0B, TIFR1, Dummy7, Dummy6, ICF1, Dummy4, Dummy3, OCF1B, OCF1A, TOV1)
SFR_B_N(0x08, ACSR, ACD, ACBG, ACO, ACI, ACIE, Dummy2, ACIS1, ACIS0)
SFR_B_N(0x07, ADMUX, REFS1, REFS0, MUX5, MUX4, MUX3, MUX2, MUX1, MUX0)
SFR_B_N(0x06, ADCSRA, ADEN, ADSC, ADATE, ADIF, ADIE, ADPS2, ADPS1, ADPS0)
//SFR_W_R(0x05, ADC)
SFR_W_R(0x04, ADC)
SFR_B_N(0x03, ADCSRB, BIN, ACME, Dummy5, ADLAR, Dummy3, ADTS2, ADTS1, ADTS0)
SFR_B_N(0x01, DIDR0, ADC7D, ADC6D, ADC5D, ADC4D, ADC3D, ADC2D, ADC1D, ADC0D)
SFR_B_N(0x00, PRR, Dummy7, Dummy6, Dummy5, Dummy4, PRTIM1, PRTIM0, PRUSI, PRADC)

#ifndef __IOTINY24_H
#define __IOTINY24_H

/* SFRs are local in assembler modules (so this file may need to be */
/* included in more than one module in the same source file), */
/* but #defines must only be made once per source file. */

/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

/* NB! vectors are specified as byte addresses */

#define     RESET_vect          (0x00)      // Reset
#define     INT0_vect           (0x02)      // External Interrupt0
#define     PCINT0_vect         (0x04)      // Pin Change Interrupt0
#define     PCINT1_vect         (0x06)      // Pin Change Interrupt1
#define     WDT_vect            (0x08)      // Watchdog Timer Interrupt
#define     TIM1_CAPT_vect      (0x0A)      // Input capture interrupt 1
#define     TIM1_COMPA_vect     (0x0C)      // Timer/Counter1 Compare Match A
#define     TIM1_COMPB_vect     (0x0E)      // Timer/Counter1 Compare Match B
#define     TIM1_OVF_vect       (0x10)      // Overflow1 Interrupt
#define     TIM0_COMPA_vect     (0x12)      // Timer/Counter0 Compare Match A
#define     TIM0_COMPB_vect     (0x14)      // Timer/Counter0 Compare Match B
#define     TIM0_OVF_vect       (0x16)      // Overflow0 Interrupt
#define     ANA_COMP_vect       (0x18)      // Analog Comparator Interrupt
#define     ADC_vect            (0x1A)      // A/D Conversion Complete
#define     EE_RDY_vect         (0x1C)      // EEPROM write complete
#define     USI_START_vect      (0x1E)      // USI start interrupt
#define     USI_OVF_vect        (0x20)      // USI overflow interrupt

        // for compatibility purpose
#define    USI_STRT_vect        USI_START_vect


#ifdef __IAR_SYSTEMS_ASM__
 #ifndef ENABLE_BIT_DEFINITIONS
  #define  ENABLE_BIT_DEFINITIONS
 #endif /* ENABLE_BIT_DEFINITIONS */
#endif /* __IAR_SYSTEMS_ASM__ */

#ifdef ENABLE_BIT_DEFINITIONS

/* Bit definitions for use with the IAR Assembler
   The Register Bit names are represented by their bit number (0-7).
*/

   /* Bit definitions for use with the IAR Assembler
      The Register Bit names are represented by their bit number (0-7).
   */

//**** SREG *******
#define SR_I       7
#define SR_T       6
#define SR_H       5
#define SR_S       4
#define SR_V       3
#define SR_N       2
#define SR_Z       1
#define SR_C       0

//**** SPH ********
#define SP9        1
#define SP8        0

//**** SPL ********
#define SP7        7
#define SP6        6
#define SP5        5
#define SP4        4
#define SP3        3
#define SP2        2
#define SP1        1
#define SP0        0

//**** GIMSK ******
#define INT0       6
#define PCIE1      5
#define PCIE0      4

//**** GIFR *******
#define INTF0      6
#define PCIF1      5
#define PCIF0      4

//**** TIMSK0 ******
#define OCIE0B     2
#define OCIE0A     1
#define TOIE0      0

//**** TIMSK1 ******
#define ICIE1      5
#define OCIE1B     2
#define OCIE1A     1
#define TOIE1      0

//**** TIFR0 *******
#define OCF0B      2
#define OCF0A      1
#define TOV0       0

//**** TIFR1 *******
#define ICIF1      5
#define OCF1B      2
#define OCF1A      1
#define TOV1       0

//**** SPMCSR *****
#define CTPB       4
#define RFLB       3
#define PGWRT      2
#define PGERS      1
#define SPMEN      0

//**** MCUCR ******
#define PUD        6
#define SE         5
#define SM1        4
#define SM0        3
#define ISC01      1
#define ISC00      0

//**** MCUSR ******
#define WDRF       3
#define BORF       2
#define EXTRF      1
#define PORF       0

//**** TCCR0B *****
#define FOC0A      7
#define FOC0B      6
#define WGM02      3
#define CS02       2
#define CS01       1
#define CS00       0

//**** OSCCAL *****
#define CAL7       7
#define CAL6       6
#define CAL5       5
#define CAL4       4
#define CAL3       3
#define CAL2       2
#define CAL1       1
#define CAL0       0

//**** TCCR0A *****
#define COM0A1     7
#define COM0A0     6
#define COM0B1     5
#define COM0B0     4
#define WGM01      1
#define WGM00      0

//**** TCCR1A *****
#define COM1A1     7
#define COM1A0     6
#define COM1B1     5
#define COM1B0     4
#define WGM11      1
#define WGM10      0
#define PWM11       1       // for compatibility purpose
#define PWM10       0       // for compatibility purpose

//**** TCCR1B *****
#define ICNC1      7
#define ICES1      6
#define WGM13      4
#define WGM12      3
#define CS12       2
#define CS11       1
#define CS10       0

//**** CLKPR ******
#define CLKPCE     7
#define CLKPS3     3
#define CLKPS2     2
#define CLKPS1     1
#define CLKPS0     0

//**** GTCCR ******
#define TSM        7
#define PSR10      0

//**** TCCR1C *****
#define FOC1A      7
#define FOC1B      6

//**** WDTCSR *****
#define WDIF       7
#define WDIE       6
#define WDP3       5
#define WDCE       4
#define WDE        3
#define WDP2       2
#define WDP1       1
#define WDP0       0
#define WDTOE       4       // for compatibility purpose

//**** PCMSK1 ******
#define PCINT11    3
#define PCINT10    2
#define PCINT9     1
#define PCINT8     0

//**** EECR *******
#define EEPM1      5
#define EEPM0      4
#define EERIE      3
#define EEMPE      2
#define EEPE       1
#define EERE       0
#define EEMWE       2       // Kept for backward compatibility
#define EEWE        1       // Kept for backward compatibility

//**** PORTA ******
#define PORTA7     7
#define PORTA6     6
#define PORTA5     5
#define PORTA4     4
#define PORTA3     3
#define PORTA2     2
#define PORTA1     1
#define PORTA0     0

//**** DDRA *******
#define DDA7       7
#define DDA6       6
#define DDA5       5
#define DDA4       4
#define DDA3       3
#define DDA2       2
#define DDA1       1
#define DDA0       0

//**** PINA *******
#define PINA7      7
#define PINA6      6
#define PINA5      5
#define PINA4      4
#define PINA3      3
#define PINA2      2
#define PINA1      1
#define PINA0      0

//**** PORTB ******
#define PORTB3     3
#define PORTB2     2
#define PORTB1     1
#define PORTB0     0

#define PB3     3
#define PB2     2
#define PB1     1
#define PB0     0

//**** DDRB *******
#define DDB3       3
#define DDB2       2
#define DDB1       1
#define DDB0       0

//**** PINB *******
#define PINB3      3
#define PINB2      2
#define PINB1      1
#define PINB0      0

//**** PCMSK0 ******
#define PCINT7     7
#define PCINT6     6
#define PCINT5     5
#define PCINT4     4
#define PCINT3     3
#define PCINT2     2
#define PCINT1     1
#define PCINT0     0

//**** USISR ******
#define    USISIF   7
#define    USIOIF   6
#define    USIPF    5
#define    USIDC    4
#define    USICNT3  3
#define    USICNT2  2
#define    USICNT1  1
#define    USICNT0  0

//**** USICR ******
#define    USISIE   7
#define    USIOIE   6
#define    USIWM1   5
#define    USIWM0   4
#define    USICS1   3
#define    USICS0   2
#define    USICLK   1
#define    USITC    0

//**** ACSR *******
#define ACD        7
#define ACBG       6
#define ACO        5
#define ACI        4
#define ACIE       3
#define ACIC       2
#define ACIS1      1
#define ACIS0      0

//**** ADMUX ******
#define REFS1      7
#define REFS0      6
#define MUX5       5
#define MUX4       4
#define MUX3       3
#define MUX2       2
#define MUX1       1
#define MUX0       0

//**** ADCSRA *****
#define ADEN       7
#define ADSC       6
#define ADATE      5
#define ADIF       4
#define ADIE       3
#define ADPS2      2
#define ADPS1      1
#define ADPS0      0

//**** ADCSRB *****
#define BIN        7
#define ACME       6
#define ADLAR      4
#define ADTS2      2
#define ADTS1      1
#define ADTS0      0

//**** DIDR0 ******
#define ADC7D      7
#define ADC6D      6
#define ADC5D      5
#define ADC4D      4
#define ADC3D      3
#define ADC2D      2
#define ADC1D      1
#define ADC0D      0

//**** PRR ********
#define PRTIM1     3
#define PRTIM0     2
#define PRUSI      1
#define PRADC      0

/* Pointer definition   */
#define    XL     r26
#define    XH     r27
#define    YL     r28
#define    YH     r29
#define    ZL     r30
#define    ZH     r31

/* Constants */
#define    RAMSTART  0x0060
#define    RAMEND    0x00DF	/*Last On-Chip SRAM location*/
#define    E2END     0x007F
#define    FLASHEND  0x07FF	/* Bytes */

//-----------------------------------------

#endif /* ENABLE_BIT_DEFINITIONS */
#endif /* __IOTINY24_INCLUDED (define part) */
#endif /* __IOTINY24_INCLUDED (SFR part) */


