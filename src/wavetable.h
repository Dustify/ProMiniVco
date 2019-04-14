// wavetable generation

#ifndef WAVETABLE_H

#define WAVETABLE_H

#include "config.h"
#include "model.h"

class WaveTable
{
  public:
    // number of amplitudes - based on number of bits available
    static double amplitudes;

    // half the number of phases - useful for calculations later
    static double halfPhases;

    // generate a sawtooth wave
    static void generate_sawtooth()
    {
        // iterate all phases
        for (uint16_t i = 0; i < COUNT_PHASE; i++)
        {
            // increment evenly to completion
            Model::wavetable[i] = ((double)i / (double)COUNT_PHASE) * amplitudes;
        }
    }

    // generate sine wave
    static void generate_sine()
    {
        // calculate amplitudes half way point
        static double halfAmplitudes = amplitudes / 2.0;

        // calculate number of radians to increment per phase
        double radiansPerPhase = (2.0 * PI) / (double)COUNT_PHASE;

        // iterate all phases
        for (uint16_t i = 0; i < COUNT_PHASE; i++)
        {
            // calculate initial value
            double sineValue = double(i) * radiansPerPhase;

            // calculate sine
            sineValue = sin(sineValue);

            // bump by '1' - this moves the wave to centre
            sineValue += 1;
            
            // multiply by half amplitudes to get final value
            sineValue *= halfAmplitudes;

            // round and store
            Model::wavetable[i] = round(sineValue);
        }
    }

    // generate triangle wave
    static void generate_triangle()
    {
        // calculate number of amplitudes per half a wave
        double amplitudesPerPhase = amplitudes / halfPhases;

        // iterate all phases
        for (uint16_t i = 0; i < COUNT_PHASE; i++)
        {
            // store current phase
            uint16_t iAdj = i;

            // if we're past the half way point
            if (iAdj >= halfPhases)
            {
                // 'flip' position to calculate for
                iAdj = COUNT_PHASE - iAdj;
            }

            // round and store
            Model::wavetable[i] = round((double)iAdj * amplitudesPerPhase);
        }
    }

    // generate square wave
    static void generate_square()
    {
        // iterate all phases
        for (uint16_t i = 0; i < COUNT_PHASE; i++)
        {
            // set value bases on 50% duty
            Model::wavetable[i] = i < halfPhases ? 0 : amplitudes;
        }
    }

    // store the current waveform 'id'
    static uint8_t waveform;

    // store debounce check
    static bool waveButtonDebounce;

    // handles a change in waveform - will cycle through available waveforms
    static void handleWaveChange(boolean buttonPressed)
    {
        // if button has not been pressed then reset debounce and stop
        if (!buttonPressed)
        {
            waveButtonDebounce = false;
            return;
        }

        // if debounced then stop
        if (waveButtonDebounce)
        {
            return;
        }

        // increment current waveform
        waveform++;

        // wrap back to start if necessary
        if (waveform > 3)
        {
            waveform = 0;
        }

        // disable ints
        noInterrupts();

        // switch current waveform and generate
        switch (waveform)
        {
        case 0:
            WaveTable::generate_sine();
            break;
        case 1:
            WaveTable::generate_triangle();
            break;
        case 2:
            WaveTable::generate_square();
            break;
        case 3:
            WaveTable::generate_sawtooth();
            break;
        }

        // enable ints
        interrupts();

        // debounce
        waveButtonDebounce = true;
    }
};

// initialise values
double WaveTable::amplitudes = 255;
double WaveTable::halfPhases = (double)COUNT_PHASE * 0.5;

uint8_t WaveTable::waveform = 0;
bool WaveTable::waveButtonDebounce = false;

#endif