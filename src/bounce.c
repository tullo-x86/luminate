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

const unsigned int bounceFrameMs = 1;

// Position is an 8.8 fixed point value.
int position = (18 << 8) + 1;
int bounceHue = 0;

int bounceFalloff(int pulsePosition, int pixelIndex)
{

	int difference = (pixelIndex << 8) - pulsePosition;

	long long falloff = ((long long)difference * difference / 1024);
	if (falloff > 255) return 0;

	return 255 - falloff;
}

void bounceRender() {
	memset(frameBuffer, 0, sizeof(struct cRGB) * NUM_LEDS);

	int initialPixel = position >> 8;
	frameBuffer[initialPixel] = hsvToRgbInt3(bounceHue, MAX_SAT, bounceFalloff(position, initialPixel));

	int nextPixel = initialPixel + 1;
	int val = bounceFalloff(position, nextPixel);
	while (val > 0) {
		frameBuffer[nextPixel % 24] = hsvToRgbInt3(bounceHue, MAX_SAT, val);
		nextPixel++;
		val = bounceFalloff(position, nextPixel);
	}

	nextPixel = initialPixel - 1;
	val = bounceFalloff(position, nextPixel);
	while (val > 0) {
		int pixelIdx = nextPixel < 0 ? nextPixel + 24 : nextPixel;
		frameBuffer[pixelIdx] = hsvToRgbInt3(bounceHue, MAX_SAT, val);
		nextPixel++;
		val = bounceFalloff(position, nextPixel);
	}

    ws2812_setleds(frameBuffer, NUM_LEDS); // Blocks for ~0.7ms
}

void bounceBegin() {
}

void bounce(unsigned long lengthMs) {

	unsigned long time = 0;
    while(time < lengthMs)
    {
    	if (++bounceHue >= MAX_HUE) bounceHue -= MAX_HUE;
    	position += 8;
    	if(position >= (24 << 8))
    		position = 0;
    	bounceRender();
        _delay_ms(bounceFrameMs);
        time += bounceFrameMs;
    }
}

void bounceEnd() {
}
