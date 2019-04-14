// defines shared config
#ifndef CONFIG_H

#define CONFIG_H

#include <Arduino.h>

// analog pin used for tuning pot
#define PIN_TUNING A5

// analog pin used for CV input
#define PIN_CV A3

// digital pin connected to button to change wavetable
#define PIN_WAVE 10

// number of phases for wavetable - this should be as high as possible without exhausting memory
#define COUNT_PHASE 1900

// factor to reduce analogRead values by
// this is effectively a frequency limit and allows for finer control of frequency
#define FACTOR 3

// sample rate Hz - controls how frequently we write to the dac
#define SAMPLE_RATE 1e6

#endif