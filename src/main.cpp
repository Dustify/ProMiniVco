#include <Arduino.h>
#include <TimerOne.h>

#define PIN_TUNING A5
#define COUNT_PHASE 2000

static uint16_t position;
static uint8_t wavetable[COUNT_PHASE];

void tick()
{
  uint16_t value = analogRead(PIN_TUNING) / 3;
  // value = 1023.0 - value; // reverse pot direction
  // value /= 1023.0;
  // value = pow(value, 2.0); // 'expo' style smoothing
  // value *= 200.0;          // frequency limiting

  position += value;

  int16_t offset = position - COUNT_PHASE;

  if (offset > 0)
  {
    position = offset;
  }

  PORTD = wavetable[position];
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_TUNING, INPUT);

  DDRD = B11111111;

  for (uint16_t i = 0; i < COUNT_PHASE; i++)
  {
    wavetable[i] = ((double)i / (double)COUNT_PHASE) * (double)255;
  }

  Timer1.initialize(1e6 / 6e3);
  Timer1.attachInterrupt(tick);
}

static uint8_t STATE_LED = LOW;

void loop()
{
  STATE_LED = !STATE_LED;

  digitalWrite(LED_BUILTIN, STATE_LED);
  delay(500);
}