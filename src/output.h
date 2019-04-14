#ifndef OUTPUT_H

#define OUTPUT_H

#include "model.h"

class Output
{
  public:
    static void Tick()
    {
        if (Model::busy)
        {
            return;
        }

        uint16_t value = (analogRead(PIN_TUNING) + analogRead(PIN_CV)) / FACTOR;

        // value = 1023.0 - value; // reverse pot direction
        // value /= 1023.0;
        // value = pow(value, 2.0); // 'expo' style smoothing
        // value *= 200.0;          // frequency limiting

        Model::position += value;

        if (Model::position >= COUNT_PHASE)
        {
            Model::position = Model::position - COUNT_PHASE;
        }

        PORTD = Model::wavetable[Model::position];
    }
};

#endif