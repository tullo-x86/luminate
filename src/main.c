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

int main()
{
    while(1)
    {
    	mesmerBegin();
    	mesmer(10000UL);
    	mesmerEnd();
    }
}
