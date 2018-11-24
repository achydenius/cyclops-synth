#include <TogglableEncoder.h>

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

TogglableEncoder envEncoder(19, 20, 0, 100, 0, 10000, 0, 10000);
TogglableEncoder lfoEncoder(17, 18, 1, 100, 0, 10, 0, 0.95);
TogglableEncoder subEncoder(15, 16, 2, 100, 215, 225, 0, 1.0);

void setup() {
  Serial.begin(9600);

  AudioMemory(10);

  osc.frequency(440);
  osc.amplitude(1.0);
  sub.frequency(220);
  sub.amplitude(1.0);

  envEncoder.setEncoderValues(1, 1000);
  lfoEncoder.setEncoderValues(5, 0.95);
  subEncoder.setEncoderValues(1.0, 220);

  envelope.decay(0);
  envelope.sustain(1.0);
}

bool playing = false;
unsigned long time;
void loop() {
  envEncoder.read();
  lfoEncoder.read();
  subEncoder.read();

  if (envEncoder.isButtonClicked()) {
    Serial.println(envEncoder.getSelectedEncoder() ? "Env: Release" : "Env: Attack");
  }

  if (lfoEncoder.isButtonClicked()) {
    Serial.println(lfoEncoder.getSelectedEncoder() ? "LFO: Amplitude" : "Env: Frequency");
  }

  if (subEncoder.isButtonClicked()) {
    Serial.println(subEncoder.getSelectedEncoder() ? "Sub: Amplitude" : "Sub: Detune");
  }

  if (envEncoder.hasEncoderValueChanged()) {
    float value = envEncoder.getEncoderValue();
    if (envEncoder.getSelectedEncoder()) {
      envelope.release(value);
    } else {
      envelope.attack(value);
    }
    Serial.println(value);
  }

  if (lfoEncoder.hasEncoderValueChanged()) {
    float value = lfoEncoder.getEncoderValue();
    if (lfoEncoder.getSelectedEncoder()) {
      lfo.amplitude(value);
    } else {
      lfo.frequency(value);
    }
    Serial.println(value);
  }
  
  if (subEncoder.hasEncoderValueChanged()) {
    float value = subEncoder.getEncoderValue();
    if (subEncoder.getSelectedEncoder()) {
      sub.amplitude(value);
    } else {
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
