// Glowbek: The doumbek that glows!
// Author: Daniel Tullemans / "LuminousElement"
// Created 2014

#include "light_ws2812.h"
#include <util/delay.h>
#include <avr/io.h>

typedef struct cRGB GRB;

int main()
{
	/////////////////////////////
	// Set up the analogue input
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 -- 125KHz sample rate @ 16MHz

	ADMUX |= (1 << REFS0); // Set reference voltage to AVCC
	ADMUX |= (1 << ADLAR); // Make the ADC left-align the values (0-255)

	ADCSRA |= (1 << ADATE); // Enable the Analog-Digital Auto Trigger Event

	ADCSRA |= (1 << ADEN); // Enable the ADC
	ADCSRA |= (1 << ADSC); // Start ADC measurements

	GRB buffer[1] = { { 2, 2, 2 } };

	while(1) {
		memset(buffer, 0, 3);

		if (ADCH > 64) buffer[0].r = ADCH;
		else if (ADCH > 8) buffer[0].g = ADCH;
		else buffer[0].b = ADCH;

		ws2812_setleds(buffer, 1);
		_delay_ms(50);
	}
}
