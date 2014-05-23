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

const unsigned int bounceFrameMs = 5;

// Position is an 8.8 fixed point value.
int position = (2 << 8) + 1;

int bounceFalloff(int pulsePosition, int pixelPosition)
{
	int difference = pixelPosition - pulsePosition;
	if (difference < 0) difference = -difference;

	int brightness = MAX_BRIGHTNESS;

	// For each full pixel the pulse is away from the pixel, falloff by 1/4.
	while(difference > 0xFF)
	{
		brightness >>= 2;
		difference -= 256;
	}

	// Now do the fraction part!

	// Linearly interpolate the last step
	int brightnessRemnant = 256 - difference;
	return brightness * brightnessRemnant / 256;
}

void bounceRender() {
	memset(frameBuffer, 0, sizeof(struct cRGB) * NUM_LEDS);
	int hue = MAX_HUE / 3 * 2;

	// Is position dead on a single pixel?
	if(1)// ((position & 0xFF) == 0)
	{
		// true:  	Centre pixel at fullbright, iterate falloff to +1/-1
		int centrePixel = position >> 8;
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
    		position = (2 << 8);
    	bounceRender();
        _delay_ms(bounceFrameMs);
        time += bounceFrameMs;
    }
}

void bounceEnd() {
}
