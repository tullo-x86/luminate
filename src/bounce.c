/*
 * bounce.c
 *
 *  Created on: 23 May 2014
 *      Author: tullo
 */

#include "bounce.h"
#include "config.h"
#include "hsv.h"
#include <string.h> /* memset */
#include <util/delay.h>

const unsigned int bounceFrameMs = 25;

// Position is an 8.8 fixed point value.
int position = (18 << 8) + 1;

int bounceFalloff(int pulsePosition, int pixelPosition)
{
	int difference = pixelPosition - pulsePosition;

	long long falloff = ((long long)difference * difference / 1024);
	if (falloff > 255) return 0;

	return 255 - falloff;
}

void bounceRender() {
	memset(frameBuffer, 0, sizeof(struct cRGB) * NUM_LEDS);
	int hue = MAX_HUE / 3 * 2;

	// Is position dead on a single pixel?
	if(1)// ((position & 0xFF) == 0)
	{
		// true:  	Centre pixel at fullbright, iterate falloff to +1/-1
		int centrePixel = position >> 8;
		if ((position & 0xFF) > 0x7F) centrePixel++;
		frameBuffer[centrePixel] = hsvToRgbInt3(hue, MAX_SAT, bounceFalloff(position, (centrePixel) << 8));

		frameBuffer[centrePixel + 1] = hsvToRgbInt3(hue, MAX_SAT, bounceFalloff(position, (centrePixel + 1) << 8));
		frameBuffer[centrePixel - 1] = hsvToRgbInt3(hue, MAX_SAT, bounceFalloff(position, (centrePixel - 1) << 8));
		frameBuffer[centrePixel + 2] = hsvToRgbInt3(hue, MAX_SAT, bounceFalloff(position, (centrePixel + 2) << 8));
		frameBuffer[centrePixel - 2] = hsvToRgbInt3(hue, MAX_SAT, bounceFalloff(position, (centrePixel - 2) << 8));
	}
	else
	{
		// false:	Iterate falloff to -1, -2 then +1, +2
	}

    ws2812_setleds(frameBuffer, NUM_LEDS); // Blocks for ~0.7ms
}

void bounceBegin() {
}

void bounce(unsigned long lengthMs) {

	unsigned long time = 0;
    while(time < lengthMs)
    {
    	position++;
    	if(position > (22 << 8))
    		position = (18 << 8);
    	bounceRender();
        _delay_ms(bounceFrameMs);
        time += bounceFrameMs;
    }
}

void bounceEnd() {
}
