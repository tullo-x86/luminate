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
unsigned char bouncePixBuf[NUM_LEDS];
#define BOUNCE_MAX_BRIGHTNESS 127
unsigned char bounceBrightness = 0;

int bounceFalloff(int pulsePosition, int pixelIndex)
{
	int difference = (pixelIndex << 8) - pulsePosition;

	long long falloff = ((long long)difference * difference / 1024);
	if (falloff > bounceBrightness) return 0;

	return bounceBrightness - falloff;
}

void bounceRenderBouncer(int position) {
	int initialPixel = position >> 8;
	assignGreater(bouncePixBuf + initialPixel, bounceFalloff(position, initialPixel));

	int nextPixel = initialPixel + 1;
	int val = bounceFalloff(position, nextPixel);
	while (val > 0) {
		assignGreater(bouncePixBuf + (nextPixel % NUM_LEDS), val);
		nextPixel++;
		val = bounceFalloff(position, nextPixel);
	}

	nextPixel = initialPixel - 1;
	val = bounceFalloff(position, nextPixel);
	while (val > 0) {
		int pixelIdx = nextPixel < 0 ? nextPixel + NUM_LEDS : nextPixel;
		assignGreater(bouncePixBuf + pixelIdx, val);
		nextPixel++;
		val = bounceFalloff(position, nextPixel);
	}
}

// Position is an 8.8 fixed point value.
int truePosition = 0;
int reflectionPosition = NUM_LEDS << 7;
int hasReflection = 1;

inline void bounceRender()
{
	memset(bouncePixBuf, 0, sizeof(unsigned char) * NUM_LEDS);
	bounceRenderBouncer(truePosition);
	if (hasReflection) bounceRenderBouncer(reflectionPosition);

	memset(frameBuffer, 0, sizeof(struct cRGB) * NUM_LEDS);

	for (int i = 0; i < NUM_LEDS; i++)
	{
		frameBuffer[i] = hsvToRgbInt3(bounceHue, MAX_SAT, bouncePixBuf[i]);
	}

    ws2812_setleds(frameBuffer, NUM_LEDS); // Blocks for ~0.7ms
}

int direction = 1;

inline void bounceMove()
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

inline void bounceLogic()
{
	if (++bounceHue >= MAX_HUE) bounceHue -= MAX_HUE;

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

inline void bounceLogicNoSplit()
{
	if (++bounceHue >= MAX_HUE) bounceHue -= MAX_HUE;

	bounceMove();
}

void bounce(unsigned long lengthMs)
{
	unsigned long time = 0;
    while(time < lengthMs
    	|| hasReflection)
    {
    	bounceLogic();
    	bounceRender();

        _delay_ms(4);
        time += 5; // wow, so divide, much cycle
    }
}

void bounceBegin() {
	bounceBrightness = 0;
    while(++bounceBrightness < BOUNCE_MAX_BRIGHTNESS)
    {
    	bounceLogic();
    	hasReflection = 0;
    	bounceRender();

        _delay_ms(4);
    }
}

void bounceEnd() {
    while(--bounceBrightness > 0)
    {
    	bounceLogicNoSplit();
    	bounceRender();

        _delay_ms(4);
    }
}
