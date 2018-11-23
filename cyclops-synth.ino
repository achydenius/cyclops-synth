#include <ButtonEncoder.h>

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

ButtonEncoder envEncoder(19, 20, 0, encoderSteps, 0);
ButtonEncoder lfoEncoder(17, 18, 0, encoderSteps, 1);
ButtonEncoder subEncoder(15, 16, 0, encoderSteps, 2);

void setup() {
  Serial.begin(9600);

  AudioMemory(10);

  osc.frequency(440);
  osc.amplitude(1.0);
  sub.frequency(220);
  sub.amplitude(1.0);

  envEncoder.setValue(1);
  lfoEncoder.setValue(5);
  subEncoder.setValue(1.0);

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

  if (envEncoder.hasValueChanged()) {
    float value = map_float(envEncoder.getValue(), 0, encoderSteps, 0, 10000);
    if (envEncoder.getButtonState()) {
      envelope.release(value);
    } else {
      envelope.attack(value);
    }
    Serial.println(value);
  }

  if (lfoEncoder.hasValueChanged()) {
    float value;
    if (lfoEncoder.getButtonState()) {
      value = map_float(lfoEncoder.getValue(), 0, encoderSteps, 0, 1.0);
      lfo.amplitude(value);
    } else {
      value = map_float(lfoEncoder.getValue(), 0, encoderSteps, 0, 10.0);
      lfo.frequency(value);
    }
    Serial.println(value);
  }
  
  if (subEncoder.hasValueChanged()) {
    float value;
    if (subEncoder.getButtonState()) {
      value = map_float(subEncoder.getValue(), 0, encoderSteps, 0, 1.0);
      sub.amplitude(value);
    } else {
      value = map_float(subEncoder.getValue(), 0, encoderSteps, 220, 210);
      sub.frequency(value);
    }
    Serial.println(value);
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
