// Glowbek: The doumbek that glows!
// Author: Daniel Tullemans / "LuminousElement"
// Created 2014

#include "light_ws2812.h"
#include <util/delay.h>
#include <avr/io.h>
#include "hsv.h"

typedef struct cRGB GRB;

int main()
{
    /////////////////////////////
    // Set up the analogue input
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // Set ADC prescaler to 128 -- 125KHz sample rate @ 16MHz

    // ADMUX:   REFS1   REFS0   Refernce
    //          0       0       External ARef
    //          0       1       Vcc
    //          1       1       Internal 1.1V (ATMega168/328) / 2.56V (ATMega8)

    ADMUX  = _BV(MUX0); // Set ADC multiplexer to read from channel 1 (PORTC 1)
    ADMUX |= _BV(REFS0) | _BV(REFS1);
    ADMUX |= _BV(ADLAR); // Make the ADC left-align the values. We then read bits 9 to 2 from ADCH (ignoring bits 0 and 1 in ADCL).

    ADCSRA |= _BV(ADATE); // Enable the Analog-Digital Auto Trigger Event

    ADCSRA |= _BV(ADEN); // Enable the ADC
    ADCSRA |= _BV(ADSC); // Start ADC measurements

    GRB frameBuffer[40];
    memset(frameBuffer, 0, 3 * 40);

    uint8_t valBuffer[40];
    memset(valBuffer, 0, 40);

    uint16_t hue = 0;

    while(1) {

        hue += 5;
        if (hue > MAX_HUE) hue = 0;

        uint16_t out = ADCH * 5 + valBuffer[39];

        for (int i = 39; i > 0; --i)
        {
            int v = valBuffer[i-1] - 2;
            valBuffer[i] = v < 0 ? 0 : v;
        }

        valBuffer[0] = (out > 255) ? 255 : out;


        for (int i=0; i<40; i++) {

            cHSV colour = { hue + i * 19, MAX_SAT, valBuffer[i] };
            frameBuffer[i] = ToRgb(&colour);
        }

        ws2812_setleds(frameBuffer, 40);
        _delay_ms(15);
    }
}
