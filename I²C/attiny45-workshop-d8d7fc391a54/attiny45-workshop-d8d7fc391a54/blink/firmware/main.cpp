#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB = _BV(PB0)|_BV(PB1);

	PORTB = _BV(PB0);	
	_delay_ms(1000);
	PORTB ^= _BV(PB0);	
	
    for(;;){
        _delay_ms(500);
        PORTB ^= _BV(PB1);
    }
    return 0;
}