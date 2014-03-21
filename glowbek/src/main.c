// Glowbek: The doumbek that glows!
// Author: Daniel Tullemans / "LuminousElement"
// Created 2014

#include "light_ws2812.h"
#include <util/delay.h>
#include <avr/io.h>
#include "hsv.h"

typedef struct cRGB GRB;

int main()
{
	/////////////////////////////
	// Set up the analogue input
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 -- 125KHz sample rate @ 16MHz

	ADMUX |= ((1 << REFS0) | (1 << REFS1)); // Set reference voltage to 2.56V
	ADMUX |= (1 << ADLAR); // Make the ADC left-align the values (0-255)

	ADCSRA |= (1 << ADATE); // Enable the Analog-Digital Auto Trigger Event

	ADCSRA |= (1 << ADEN); // Enable the ADC
	ADCSRA |= (1 << ADSC); // Start ADC measurements

	GRB frameBuffer[40];
	memset(frameBuffer, 0, 3 * 40);

	uint8_t valBuffer[40];
	memset(valBuffer, 0, 40);

	uint16_t hue = 0;


	while(1) {

		hue += 5;
		if (hue > MAX_HUE) hue = 0;

		uint16_t out = ADCH * 5 + valBuffer[39];

		for (int i = 39; i > 0; --i)
		{
			int v = valBuffer[i-1] - 2;
			valBuffer[i] = v < 0 ? 0 : v;
		}

		valBuffer[0] = (out > 255) ? 255 : out;


		for (int i=0; i<40; i++) {

			cHSV colour = { hue + i * 19, MAX_SAT, valBuffer[i] };
			frameBuffer[i] = ToRgb(&colour);
		}

		ws2812_setleds(frameBuffer, 40);
		_delay_ms(15);
	}
}
