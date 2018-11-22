#include <LimitedEncoder.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   lfo;           //xy=228,330
AudioSynthWaveformPWM    sub;           //xy=434,366
AudioSynthWaveformPWM    osc;           //xy=435,308
AudioOutputI2S           i2s;           //xy=578,227
AudioMixer4              mixer;         //xy=595,340
AudioEffectEnvelope      envelope;      //xy=780,325
AudioOutputUSB           usb;           //xy=955,386
AudioConnection          patchCord1(lfo, osc);
AudioConnection          patchCord2(lfo, sub);
AudioConnection          patchCord3(sub, 0, mixer, 1);
AudioConnection          patchCord4(osc, 0, mixer, 0);
AudioConnection          patchCord5(mixer, envelope);
AudioConnection          patchCord6(envelope, 0, usb, 0);
// GUItool: end automatically generated code

const int encoderSteps = 100;

LimitedEncoder envEncoder(19, 20, 0, 0, encoderSteps);
LimitedEncoder lfoEncoder(17, 18, 1, 0, encoderSteps);
LimitedEncoder subEncoder(15, 16, 2, 0, encoderSteps);

float envAttackValue = 0;
float envReleaseValue = 0;
float lfoFreqValue = 0;
float lfoAmpValue = 0;
float subFreqValue = 0;
float subAmpValue = 0;

void setup() {
  Serial.begin(9600);

  AudioMemory(10);

  osc.frequency(440);
  osc.amplitude(1.0);
  sub.frequency(220);
  sub.amplitude(1.0);

  envEncoder.write(1);
  lfoEncoder.write(5);
  subEncoder.write(1.0);

  envelope.decay(0);
  envelope.sustain(1.0);
}

bool playing = false;
unsigned long time;
void loop() {
  envEncoder.update();
  lfoEncoder.update();
  subEncoder.update();

  if (envEncoder.isButtonClicked()) {
    Serial.println(envEncoder.getButtonState() ? "Env: Release" : "Env: Attack");
  }

  if (lfoEncoder.isButtonClicked()) {
    Serial.println(lfoEncoder.getButtonState() ? "LFO: Amplitude" : "Env: Frequency");
  }

  if (subEncoder.isButtonClicked()) {
    Serial.println(subEncoder.getButtonState() ? "Sub: Amplitude" : "Sub: Detune");
  }

  float envValue = map_float(envEncoder.read(), 0, encoderSteps, 0, 10000);
  if (envEncoder.getButtonState() && envValue != envReleaseValue) {
    envelope.release(envValue);
    envReleaseValue = envValue;
    Serial.println(envReleaseValue);
  } else if (!envEncoder.getButtonState() && envValue != envAttackValue) {
    envelope.attack(envValue);
    envAttackValue = envValue;
    Serial.println(envAttackValue);
  }

  if (lfoEncoder.getButtonState()) {
    float lfoValue = map_float(lfoEncoder.read(), 0, encoderSteps, 0, 1.0);
    if (lfoValue != lfoAmpValue) {
      lfo.amplitude(lfoValue);
      lfoAmpValue = lfoValue;
      Serial.println(lfoAmpValue);
    }
  } else {
    float lfoValue = map_float(lfoEncoder.read(), 0, encoderSteps, 0, 10.0);
    if (lfoValue != lfoFreqValue) {
      lfo.frequency(lfoValue);
      lfoFreqValue = lfoValue;
      Serial.println(lfoFreqValue);
    }
  }

  if (subEncoder.getButtonState()) {
    float subValue = map_float(subEncoder.read(), 0, encoderSteps, 0, 1.0);
    if (subValue != subAmpValue) {
      sub.amplitude(subValue);
      subAmpValue = subValue;
      Serial.println(subAmpValue);
    }
  } else {
    float subValue = map_float(subEncoder.read(), 0, encoderSteps, 220, 210);
    if (subValue != subFreqValue) {
      sub.frequency(subValue);
      subFreqValue = subValue;
      Serial.println(subFreqValue);
    }
  }

  unsigned long t = millis();
  if (t - time > 2000) {
    if (playing) {
      envelope.noteOff();
    } else {
      envelope.noteOn();
    }
    playing = !playing;
    time = t;
  }

  delay(30);
}

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
