// pulse.h
#ifndef __PULSE_H__
#define __PULSE_H__

#include "hsv.h"

struct cPulse {
    cHSV colour;
    uint8_t position;
};

void pulseSetFrameBuffer(struct cRGB framebuffer[], uint8_t length);
void pulseClearFrameBuffer();

void pulseUpdate(struct cPulse *pulse);
void pulseRender(struct cPulse *pulse);

#endif
