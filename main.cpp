/*
 * Generate a high (~10kHz) modulated PWM signal
 * using Timer1
 */
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

// Arduino pin 9, OC1A
// Output channel A from Timer1 in PWM mode.
#define PIN 1

/* timer input frequency when using clock scale of CS11 (i.e. /8) */
#define TIMER_FREQ 2000000

int us_in_ticks(int us)
{
	return TIMER_FREQ / 1000000  * us;
}


ISR(TIMER1_COMPA_vect)
{
	static int8_t pwmval=0;
	static bool going_up=true;

	/* NB: "phase correct PWM" did do exactly this ramp-up-ramp-down.
	 * But we use "fast PWM" because.
	 * OCR1A is the comparator value register where the output gets cleared.
	 * I.e. this is the PWM duty cycle value, not the Timer1 wrap-around value*/
	if( going_up ) {
		OCR1A = us_in_ticks(pwmval++);
		if( pwmval > 40)
			going_up=false;
	}
	else{
		OCR1A = us_in_ticks(pwmval--);
		if(pwmval <= 0)
			going_up=true;
	}
}

int main(void)
{
	cli();
	/* Set OC1A pin as output */
	DDRB=_BV(PIN);

	// Use Timer1 as we can adjust the PWM period (via ICR1)
	// and duty cycle (via OCR1A) independently

	// Clear OC0A on compare match, Set at BOTTOM (COM0Ax)
	// Fast PWM mode, with wrap-around at ICR1 (WGM mode 14)
	TCCR1A = ( _BV(COM1A1) | _BV(COM1A1) | _BV(WGM11)  );

	// CS11 = timer frec = clk_io/8
	// Fast PVM mode, cont'd. (WGM12, WGM13)
	TCCR1B = ( _BV(CS11) | _BV(WGM13)| _BV(WGM12));

	// interrupt on compare match - so we can change
	// pulse duty cycle on each pulse
	TIMSK1 = _BV(OCIE1A);

	// Pulse period - i.e. timer reset value
	ICR1  = us_in_ticks(80);

	sei();
	while(1);

	return 0;
}
