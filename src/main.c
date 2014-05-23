// Luminate: generic lighting doohickey
// Author: Daniel Tullemans / "LuminousElement"
// Created 2014

#include "light_ws2812.h"
#include <string.h> /* memset */
#include <util/delay.h>
#include <avr/io.h>
#include "hsv.h"
#include "pulse.h"
#include "config.h"
#include "mesmer.h"
#include "bounce.h"

int main()
{
	memset(frameBuffer, 0, sizeof(struct cRGB) * NUM_LEDS);
    ws2812_setleds(frameBuffer, NUM_LEDS); // Blocks for ~0.7ms

    while(1) {
    	bounce(10000UL);
    }

/*
    while(1) {}
    {
    	mesmerBegin();
    	mesmer(10000UL);
    	mesmerEnd();
    }*/
}
