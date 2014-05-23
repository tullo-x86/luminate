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

	int hueOffset = 0;
    while(1)
    {
    	if (hueOffset < 0) hueOffset += MAX_HUE;
    	for (int i=0; i<NUM_LEDS; i++)
    	{
    		struct cRGB rgb = hsvToRgbInt3(hueOffset + hueStep * i, MAX_SAT, MAX_VAL / 8);
    		memcpy(frameBuffer+i, &rgb, sizeof(struct cRGB));
    	}
    	hueOffset--;
        ws2812_setleds(frameBuffer, NUM_LEDS); // Blocks for ~0.7ms
        _delay_ms(50);
    }
}
