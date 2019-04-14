// handles output of wavetable values to DAC

#ifndef OUTPUT_H

#define OUTPUT_H

#include "model.h"

class Output
{
  public:
    // executes on each 'tick' - based on samplerate
    static void Tick()
    {
        // if wavetable generation is taking place then do nothing
        if (Model::busy)
        {
            return;
        }

        // read values from analog pins and divide by factor
        uint16_t value = (analogRead(PIN_TUNING) + analogRead(PIN_CV)) / FACTOR;

        // increment position based on value
        Model::position += value;

        // if position has exceeded number of phases
        if (Model::position >= COUNT_PHASE)
        {
            // ...wrap back around
            Model::position = Model::position - COUNT_PHASE;
        }

        // send value to DAC
        PORTD = Model::wavetable[Model::position];
    }
};

#endif