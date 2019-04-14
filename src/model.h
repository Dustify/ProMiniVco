#ifndef MODEL_H

#define MODEL_H

#include "config.h"

class Model
{
  public:
    static bool busy;
    volatile static uint16_t position;
    static uint8_t wavetable[COUNT_PHASE];
};

bool Model::busy;
volatile uint16_t Model::position;
uint8_t Model::wavetable[COUNT_PHASE];

#endif