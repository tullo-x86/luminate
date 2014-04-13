// Glowbek: The doumbek that glows!
// Author: Daniel Tullemans / "LuminousElement"
// Created 2014

#include "light_ws2812.h"
#include <string.h> /* memset */
#include <util/delay.h>
#include <avr/io.h>
#include "hsv.h"
#include "pulse.h"

typedef struct cRGB cRGB;

void setupAdcOnPin(uint8_t pin) {
    // TODO: assert pin <= 7

    DDRC &= 0xC0;  // Set ADC pins as inputs
    PORTC &= 0xC0; // Set ADC pins to not use internal pull-up
    DIDR0 &= 0x3F; // Disable digital input buffers for all ADC pins
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // Set ADC prescaler to 128 -- 125KHz sample rate @ 16MHz

    ADMUX  = 0x7 & pin; // Set ADC multiplexer to read from the given channel

    // ADMUX:   REFS1   REFS0   Refernce
    //          0       0       External ARef
    //          0       1       Vcc
    //          1       1       Internal 1.1V (ATMega168/328) / 2.56V (ATMega8)

    ADMUX |= /*_BV(REFS0) |*/ _BV(REFS1);
    ADMUX |= _BV(ADLAR); // Make the ADC left-align the values. We then read bits 9 to 2 from ADCH (ignoring bits 0 and 1 in ADCL).

    ADCSRA |= _BV(ADEN); // Power up the ADC
}

inline void adcStartConversion() {
    ADCSRA |= _BV(ADSC); // Start ADC measurements
}

cRGB frameBuffer[40];

int main()
{
    /////////////////////////////
    // Set up the analogue input
    setupAdcOnPin(0);

    adcStartConversion(); // Start ADC measurement

    pulseSetFrameBuffer(frameBuffer, 40);

    const uint8_t pulseCount = 10;
    uint8_t pulseIdx = 0;
    struct cPulse pulses[pulseCount];
    memset(pulses, 0, sizeof(struct cPulse) * pulseCount);
    pulses[0].colour.s = MAX_SAT;
    pulses[0].colour.v = MAX_VAL;

    pulses[1].position = 20;
    pulses[1].colour.h = MAX_HUE / 2;
    pulses[1].colour.s = MAX_SAT;
    pulses[1].colour.v = MAX_VAL;


    while(1) {
        // NB: Reading ADCH here assumes that we started ADC conversion at least 192
        //     clocks ago (or 1,728 clocks if it was the first conversion after
        //     powering up the ADC. If we can't be sure of that, we should wait for
        //     the ADIF bit in ADCSRA to be set.
        while (!(ADCSRA & _BV(ADIF)));

        pulseClearFrameBuffer();
        for (int i = 0; i < pulseCount; ++i) {
            pulseUpdate(pulses + i);
            pulseRender(pulses + i);
        }

        ws2812_setleds(frameBuffer, 40);
        _delay_ms(2);         // The sensor line will be noisy for a little while
        adcStartConversion(); // Start ADC measurement
        _delay_ms(13);
    }
}
