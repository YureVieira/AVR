#include <avr/io.h>
#include <util/delay.h>


int analogRead()
{
	//uint8_t low, high;

	ADCSRA |= _BV(ADSC) | _BV(ADEN);
	while ((ADCSRA & _BV(ADSC)) != 0);
	
	// start the conversion
	ADCSRA |= _BV(ADSC);
	
	// ADSC is cleared when the conversion finishes
	PORTB ^= _BV(1);	
	while ((ADCSRA & _BV(ADSC)) != 0);
	PORTB ^= _BV(1);	
	
	ADCSRA ^= _BV(ADEN);
	
	return ADC>>2;
	//return ADC;
}

int main(void)
{
	// TODO use PB1, PB2 macros
    DDRB = _BV(0)|_BV(1);           /* make the LED pin an output */
	
	// init ADC
	ADMUX = /*_BV(REFS0) |*/ /*_BV(REFS1) |*/ /*_BV(REFS2) |*/ _BV(MUX1)| _BV(MUX0); // Mux 3 ADC3 (pin 2)
	ADCSRA = /*_BV(ADEN) |*/ _BV(ADPS2)| _BV(ADPS1)| _BV(ADPS0) /*| _BV(ADATE)*//*| _BV(ADLAR)*/;
	
	
	PORTB = _BV(1);	
	_delay_ms(1000);
	PORTB ^= _BV(1);
	
    for(;;){
		int delay = analogRead();
        _delay_ms(/*50+*/delay);
        PORTB ^= _BV(0);  
    }
    return 0;
}