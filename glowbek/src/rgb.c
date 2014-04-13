// rgb.c
#include "rgb.h"

void rgbAdd(struct cRGB *addTo, struct cRGB *add)
{
	uint16_t
	    r = addTo->r + add->r,
		g = addTo->g + add->g,
		b = addTo->b + add->b;

	addTo->r = r > 255 ? 255 : r;
	addTo->g = g > 255 ? 255 : g;
	addTo->b = b > 255 ? 255 : b;
}