// shared objects

#ifndef MODEL_H

#define MODEL_H

#include "config.h"

class Model
{
  public:
    // defines whether or not wavetable generation is taking place
    static bool busy;

    // defines current position in wavetable
    volatile static uint16_t position;

    // defines wavetable
    static uint8_t wavetable[COUNT_PHASE];
};

// initialise
bool Model::busy;
volatile uint16_t Model::position;
uint8_t Model::wavetable[COUNT_PHASE];

#endif