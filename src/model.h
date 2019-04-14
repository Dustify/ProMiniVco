// shared objects

#ifndef MODEL_H

#define MODEL_H

#include "config.h"

class Model
{
  public:
    // defines current position in wavetable
    volatile static uint16_t position;

    // defines wavetable
    static uint8_t wavetable[COUNT_PHASE];
};

// initialise
volatile uint16_t Model::position;
uint8_t Model::wavetable[COUNT_PHASE];

#endif