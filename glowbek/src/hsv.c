/*
 * hsv.c
 *
 *  Created on: 31 Mar 2014
 *      Author: tullo
 */

#include "hsv.h"

const uint8_t MAX_VAL  = 255;
const uint8_t MAX_SAT  = 128;
const uint16_t MAX_HUE = 764;

cHSV RshV(cHSV *orig, uint8_t bits) {
    cHSV adjusted = { orig->h, orig->s, orig->v >> bits };
    return adjusted;
}

cRGB ToRgb(cHSV *hsv) {

    int16_t hue = hsv->h;
    const int16_t sat = hsv->s,
                  bri = hsv->v;

    int16_t red_val, green_val, blue_val;

    while (hue > 764) hue -= 765;
    while (hue < 0) hue += 765;

    if (hue < 255) {
        red_val = (10880 - sat * (hue - 170)) >> 7;
        green_val = (10880 - sat * (85 - hue)) >> 7;
        blue_val = (10880 - sat * 85) >> 7;
    }
    else if (hue < 510) {
        red_val = (10880 - sat * 85) >> 7;
        green_val = (10880 - sat * (hue - 425)) >> 7;
        blue_val = (10880 - sat * (340 - hue)) >> 7;
    }
    else {
        red_val = (10880 - sat * (595 - hue)) >> 7;
        green_val = (10880 - sat * 85) >> 7;
        blue_val = (10880 - sat * (hue - 680)) >> 7;
    }

    cRGB outRgb;

    outRgb.r = (uint16_t)((bri + 1) * red_val) >> 8;
    outRgb.g = (uint16_t)((bri + 1) * green_val) >> 8;
    outRgb.b = (uint16_t)((bri + 1) * blue_val) >> 8;

    return outRgb;
}
