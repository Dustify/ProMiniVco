// main entrypoint

#include "config.h"
#include "wavetable.h"
#include "output.h"

#include <TimerOne.h>

void setup()
{
  // set pinmodes
  // output LED
  pinMode(LED_BUILTIN, OUTPUT);
  
  // tuning pin
  pinMode(PIN_TUNING, INPUT);

  // CV pin
  pinMode(PIN_CV, INPUT);

  // wavetable cycle button
  pinMode(PIN_WAVE, INPUT_PULLUP);

  // set DAC port to all output
  DDRD = B11111111;

  // initialise wavetable - default sine
  WaveTable::generate_sine();

  // setup interrupt timer
  Timer1.initialize(1e6 / SAMPLE_RATE);
  Timer1.attachInterrupt(Output::Tick);
}

void loop()
{
  // check if wavetable cycle button has been pressed
  bool waveChangeState = digitalRead(PIN_WAVE) == LOW;

  // ...illuminate LED if pressed
  digitalWrite(LED_BUILTIN, waveChangeState ? HIGH : LOW);

  // pass button state to handler
  WaveTable::handleWaveChange(waveChangeState);
}