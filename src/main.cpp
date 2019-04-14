#include <Arduino.h>
#include <TimerOne.h>

#define PIN_TUNING A5
#define COUNT_PHASE 2000
#define FACTOR 3
#define SAMPLE_RATE 6000
#define LED_TIME_MS 500

static uint16_t position;
static uint8_t wavetable[COUNT_PHASE];

void tick()
{
  uint16_t value = analogRead(PIN_TUNING) / FACTOR;
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

void generate_sawtooth()
{
  for (uint16_t i = 0; i < COUNT_PHASE; i++)
  {
    wavetable[i] = ((double)i / (double)COUNT_PHASE) * (double)255;
  }
}

void generate_sine()
{
  double dHalfAmplitudes = 255.0 / 2.0;
  double radiansPerPhase = (2.0 * PI) / (double)COUNT_PHASE;

  for (uint16_t i = 0; i < COUNT_PHASE; i++)
  {
    double sineValue = double(i) * radiansPerPhase;
    sineValue = sin(sineValue);
    sineValue += 1;
    sineValue *= dHalfAmplitudes;
    
    wavetable[i] = round(sineValue);
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_TUNING, INPUT);

  DDRD = B11111111;

  // generate_sawtooth();
  generate_sine();

  Timer1.initialize(1e6 / SAMPLE_RATE);
  Timer1.attachInterrupt(tick);
}

static uint8_t STATE_LED = LOW;

void loop()
{
  STATE_LED = !STATE_LED;

  digitalWrite(LED_BUILTIN, STATE_LED);
  delay(LED_TIME_MS);
}