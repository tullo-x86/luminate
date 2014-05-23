#include "mesmer.h"
#include "config.h"
#include "hsv.h"
#include <util/delay.h>

const unsigned char mesmerFrameMs = 50;

int hueOffset = 0;
unsigned char brightness = 0;

void mesmerRotateHue()
{
	if (--hueOffset < 0) hueOffset += MAX_HUE;
}

void mesmerRender()
{
	const int hueStep = MAX_HUE / NUM_LEDS;
	for (int i = 0; i < NUM_LEDS; i++)
	{
		frameBuffer[i] = hsvToRgbInt3(hueOffset + hueStep * i, MAX_SAT, brightness);
	}
    ws2812_setleds(frameBuffer, NUM_LEDS); // Blocks for ~0.7ms
}

void mesmerBegin()
{
	brightness = 0;
	while(++brightness < MAX_BRIGHTNESS) {
    	mesmerRotateHue();
		mesmerRender();
        _delay_ms(mesmerFrameMs);
	}
}

void mesmer(unsigned long lengthMs)
{
	unsigned long time = 0;
    while(time < lengthMs)
    {
    	mesmerRotateHue();
    	mesmerRender();
        _delay_ms(mesmerFrameMs);
        time += mesmerFrameMs;
    }
}

void mesmerEnd()
{
	while(--brightness) {
    	mesmerRotateHue();
		mesmerRender();
        _delay_ms(mesmerFrameMs);
	}
}
