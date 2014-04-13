#ifndef __HSV_H__
#define __HSV_H__

#include "light_ws2812.h"
#include <math.h>

// 16-bit HSV colour.
// Acceptable ranges are
// h: 0 - 764
// s: 0 - 128
// v: 0 - 255
extern const uint8_t MAX_VAL;
extern const uint8_t MAX_SAT;
extern const uint16_t MAX_HUE;

typedef struct HSV {
    int16_t h;
    int16_t s;
    int16_t v;

} cHSV;

typedef struct cRGB cRGB;

cHSV RshV(cHSV *orig, uint8_t bits);

void hsvDarkenLinear(cHSV *hsv, uint8_t amount);
void hsvDarkenFalloff(cHSV *hsv);

cRGB hsvToRgb(cHSV *hsv);
cRGB hsvToRgbInt3(int16_t hue, int16_t sat, int16_t bri);

#endif
