#include <Arduino.h>

#define PIN_TUNING A5
#define COUNT_PHASE 2000

static uint16_t position;
static uint8_t wavetable[COUNT_PHASE];

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_TUNING, INPUT);

  DDRD = B11111111;

  for (uint16_t i = 0; i < COUNT_PHASE; i++)
  {
    wavetable[i] = ((double)i / (double)COUNT_PHASE) * (double)255;
  }
}

void loop()
{
  uint16_t value = (1023 - analogRead(PIN_TUNING)) / 2;
  position += value;

  int16_t offset = position - COUNT_PHASE;

  if (offset > 0) {
    position = offset;
  }

  PORTD = wavetable[position];
}