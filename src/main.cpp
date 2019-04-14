#include <Arduino.h>
#include <TimerOne.h>

#define PIN_TUNING A5
#define PIN_CV A3
#define PIN_WAVE 10
#define COUNT_PHASE 1900
#define FACTOR 3
#define SAMPLE_RATE 6000

static bool busy = false;
volatile static uint16_t position;
static uint8_t wavetable[COUNT_PHASE];

void tick()
{
  if (busy)
  {
    return;
  }

  uint16_t value = (analogRead(PIN_TUNING) + analogRead(PIN_CV)) / FACTOR;

  // value = 1023.0 - value; // reverse pot direction
  // value /= 1023.0;
  // value = pow(value, 2.0); // 'expo' style smoothing
  // value *= 200.0;          // frequency limiting

  position += value;

  if (position >= COUNT_PHASE)
  {
    position = position - COUNT_PHASE;
  }

  PORTD = wavetable[position];
}

static double amplitudes = 255;
static double halfPhases = (double)COUNT_PHASE * 0.5;

void generate_sawtooth()
{
  for (uint16_t i = 0; i < COUNT_PHASE; i++)
  {
    wavetable[i] = ((double)i / (double)COUNT_PHASE) * amplitudes;
  }
}

void generate_sine()
{
  static double halfAmplitudes = amplitudes / 2.0;
  double radiansPerPhase = (2.0 * PI) / (double)COUNT_PHASE;

  for (uint16_t i = 0; i < COUNT_PHASE; i++)
  {
    double sineValue = double(i) * radiansPerPhase;
    sineValue = sin(sineValue);
    sineValue += 1;
    sineValue *= halfAmplitudes;

    wavetable[i] = round(sineValue);
  }
}

void generate_triangle()
{
  double amplitudesPerPhase = amplitudes / halfPhases;

  for (uint16_t i = 0; i < COUNT_PHASE; i++)
  {
    uint16_t iAdj = i;

    if (iAdj >= halfPhases)
    {
      iAdj = COUNT_PHASE - iAdj;
    }

    wavetable[i] = round((double)iAdj * amplitudesPerPhase);
  }
}

void generate_square()
{
  for (uint16_t i = 0; i < COUNT_PHASE; i++)
  {
    wavetable[i] = i < halfPhases ? 0 : amplitudes;
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_TUNING, INPUT);
  pinMode(PIN_CV, INPUT);
  pinMode(PIN_WAVE, INPUT_PULLUP);

  DDRD = B11111111;

  generate_sine();

  Timer1.initialize(1e6 / SAMPLE_RATE);
  Timer1.attachInterrupt(tick);
}

static uint8_t waveform = 0;
static bool waveButtonDebounce = false;

void handleWaveChange(boolean buttonPressed)
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

  busy = true;

  switch (waveform)
  {
  case 0:
    generate_sine();
    break;
  case 1:
    generate_triangle();
    break;
  case 2:
    generate_square();
    break;
  case 3:
    generate_sawtooth();
    break;
  }

  busy = false;

  waveButtonDebounce = true;
}

void loop()
{
  bool waveChangeState = digitalRead(PIN_WAVE) == LOW;
  digitalWrite(LED_BUILTIN, waveChangeState ? HIGH : LOW);

  handleWaveChange(waveChangeState);
}