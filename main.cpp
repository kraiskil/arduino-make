#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#define RED 3
#define GREEN 4
int main(void)
{
	ADMUX = 0 // _BV(ADLAR) // left-adjust result
		|_BV(REFS0);// AVcc as refrence voltage, read
	                    // channel 0
	ADCSRA = _BV(ADEN)  // enable
		|_BV(ADPS2) // presacaler 111
		|_BV(ADPS1) // i.e. 128, i.e. 
		|_BV(ADPS0);// slow conversion - best quality
	//ADSSRB = 0 -> freerunning conversion mode

	DDRB = _BV(3)|_BV(4);
	PORTB = _BV(3)|_BV(4);

	while(1) {
		int hi, lo;
		// Trigger conversion
		ADCSRA |= _BV(ADSC);
		// wait for conversion done
		while( ADCSRA & _BV(ADSC) )
			;
		// Read ADCL first, or the result is discared
		lo = ADCL;
		hi = ADCH;
	
		printf("\nADCL = %d\n", lo);
		printf("ADCH = %d\n", hi);

		if( hi )
			PORTB=_BV(GREEN);
		else
			PORTB=_BV(RED);
		_delay_ms(1000);
	}
	return 0;
}
