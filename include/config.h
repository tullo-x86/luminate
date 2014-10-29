/*
 * config.h
 *
 *  Created on: 23 May 2014
 *      Author: tullo
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "rgb.h"

#define NUM_LEDS 32
#define MAX_BRIGHTNESS 64

inline void assignGreater(unsigned char *storage, unsigned char comparison)
{
	if (comparison > *storage) *storage = comparison;
}

// Shouldn't modify below this line

struct cRGB frameBuffer[NUM_LEDS];

#endif /* CONFIG_H_ */
