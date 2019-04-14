#include "config.h"
#include "wavetable.h"
#include "output.h"

#include <TimerOne.h>

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_TUNING, INPUT);
  pinMode(PIN_CV, INPUT);
  pinMode(PIN_WAVE, INPUT_PULLUP);

  DDRD = B11111111;

  WaveTable::generate_sine();

  Timer1.initialize(1e6 / SAMPLE_RATE);
  Timer1.attachInterrupt(Output::Tick);
}

void loop()
{
  bool waveChangeState = digitalRead(PIN_WAVE) == LOW;
  digitalWrite(LED_BUILTIN, waveChangeState ? HIGH : LOW);

  WaveTable::handleWaveChange(waveChangeState);
}