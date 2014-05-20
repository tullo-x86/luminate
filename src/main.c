// Luminate: generic lighting doohickey
// Author: Daniel Tullemans / "LuminousElement"
// Created 2014

#include "light_ws2812.h"
#include <string.h> /* memset */
#include <util/delay.h>
#include <avr/io.h>
#include "hsv.h"
#include "pulse.h"

typedef struct cRGB cRGB;

#define NUM_LEDS 24
cRGB frameBuffer[NUM_LEDS];

int main()
{
	memset(frameBuffer, 0, NUM_LEDS * 3);

	const int hueStep = MAX_HUE / NUM_LEDS;
	for (int i=0; i<NUM_LEDS; i++)
	{
		struct cRGB rgb = hsvToRgbInt3(hueStep * i, MAX_SAT, MAX_VAL);
	}

    while(1)
    {
        ws2812_setleds(frameBuffer, 40); // Blocks for ~1.2ms
    }
}
