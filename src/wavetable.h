#ifndef WAVETABLE_H

#define WAVETABLE_H

#include "config.h"
#include "model.h"

class WaveTable
{
  public:
    static double amplitudes;
    static double halfPhases;

    static void generate_sawtooth()
    {
        for (uint16_t i = 0; i < COUNT_PHASE; i++)
        {
            Model::wavetable[i] = ((double)i / (double)COUNT_PHASE) * amplitudes;
        }
    }

    static void generate_sine()
    {
        static double halfAmplitudes = amplitudes / 2.0;
        double radiansPerPhase = (2.0 * PI) / (double)COUNT_PHASE;

        for (uint16_t i = 0; i < COUNT_PHASE; i++)
        {
            double sineValue = double(i) * radiansPerPhase;
            sineValue = sin(sineValue);
            sineValue += 1;
            sineValue *= halfAmplitudes;

            Model::wavetable[i] = round(sineValue);
        }
    }

    static void generate_triangle()
    {
        double amplitudesPerPhase = amplitudes / halfPhases;

        for (uint16_t i = 0; i < COUNT_PHASE; i++)
        {
            uint16_t iAdj = i;

            if (iAdj >= halfPhases)
            {
                iAdj = COUNT_PHASE - iAdj;
            }

            Model::wavetable[i] = round((double)iAdj * amplitudesPerPhase);
        }
    }

    static void generate_square()
    {
        for (uint16_t i = 0; i < COUNT_PHASE; i++)
        {
            Model::wavetable[i] = i < halfPhases ? 0 : amplitudes;
        }
    }

    static uint8_t waveform;
    static bool waveButtonDebounce;

    static void handleWaveChange(boolean buttonPressed)
    {
        if (!buttonPressed)
        {
            waveButtonDebounce = false;
            return;
        }

        if (waveButtonDebounce)
        {
            return;
        }

        waveform++;

        if (waveform > 3)
        {
            waveform = 0;
        }

        Model::busy = true;

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

        Model::busy = false;

        waveButtonDebounce = true;
    }
};

double WaveTable::amplitudes = 255;
double WaveTable::halfPhases = (double)COUNT_PHASE * 0.5;

uint8_t WaveTable::waveform = 0;
bool WaveTable::waveButtonDebounce = false;

#endif