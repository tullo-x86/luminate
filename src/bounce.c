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
#include <stdlib.h>

int bounceHue = 0;

int bounceFalloff(int pulsePosition, int pixelIndex)
{
	int difference = (pixelIndex << 8) - pulsePosition;

	long long falloff = ((long long)difference * difference / 1024);
	if (falloff > 127) return 0;

	return 127 - falloff;
}

void bounceRender(int position) {
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
}

void bounceBegin() {
}

const unsigned int bounceFrameMs = 2;

// Position is an 8.8 fixed point value.
int truePosition = 0;
int reflectionPosition = NUM_LEDS << 7;

int direction = 1;
int hasReflection = 1;

void bounceMove()
{
	truePosition += 8 * direction;
	if(truePosition >= (NUM_LEDS << 8))
		truePosition -= (NUM_LEDS << 8);
	else if(truePosition <= 0)
		truePosition += (NUM_LEDS << 8);

	reflectionPosition -= 8 * direction;
	if(reflectionPosition >= (NUM_LEDS << 8))
		reflectionPosition -= (NUM_LEDS << 8);
	else if(reflectionPosition <= 0)
		reflectionPosition += (NUM_LEDS << 8);
}

void bounceLogic()
{
	if(hasReflection == 0)
	{
		hasReflection = (rand() & 0xFF) == 0xFF;
		if (hasReflection) reflectionPosition = truePosition;
	}
	else if (truePosition == reflectionPosition)
	{
		hasReflection = 0;
		if((rand() & 0x3) == 0x3) direction *= -1;
	}

	bounceMove();
}

void bounce(unsigned long lengthMs)
{
	unsigned long time = 0;
    while(time < lengthMs)
    {
    	if (++bounceHue >= MAX_HUE) bounceHue -= MAX_HUE;

    	bounceLogic();

    	memset(frameBuffer, 0, sizeof(struct cRGB) * NUM_LEDS);
    	bounceRender(truePosition);
    	if (hasReflection) bounceRender(reflectionPosition);

        ws2812_setleds(frameBuffer, NUM_LEDS); // Blocks for ~0.7ms

        _delay_ms(bounceFrameMs);
        time += bounceFrameMs;
    }
}

void bounceEnd() {
}
