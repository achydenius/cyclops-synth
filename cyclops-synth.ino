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

ButtonEncoder envEncoder(19, 20, 0, encoderSteps, 0, 10000, 0, 10000);
ButtonEncoder lfoEncoder(17, 18, 1, encoderSteps, 0, 10, 0, 0.95);
ButtonEncoder subEncoder(15, 16, 2, encoderSteps, 215, 225, 0, 1.0);

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
    Serial.println(envEncoder.getButtonState() ? "Env: Release" : "Env: Attack");
  }

  if (lfoEncoder.isButtonClicked()) {
    Serial.println(lfoEncoder.getButtonState() ? "LFO: Amplitude" : "Env: Frequency");
  }

  if (subEncoder.isButtonClicked()) {
    Serial.println(subEncoder.getButtonState() ? "Sub: Amplitude" : "Sub: Detune");
  }

  if (envEncoder.hasEncoderValueChanged()) {
    if (envEncoder.getButtonState()) {
      envelope.release(envEncoder.getEncoderValue());
    } else {
      envelope.attack(envEncoder.getEncoderValue());
    }
    Serial.println(envEncoder.getEncoderValue());
  }

  if (lfoEncoder.hasEncoderValueChanged()) {
    if (lfoEncoder.getButtonState()) {
      lfo.amplitude(lfoEncoder.getEncoderValue());
    } else {
      lfo.frequency(lfoEncoder.getEncoderValue());
    }
    Serial.println(lfoEncoder.getEncoderValue());
  }
  
  if (subEncoder.hasEncoderValueChanged()) {
    if (subEncoder.getButtonState()) {
      sub.amplitude(subEncoder.getEncoderValue());
    } else {
      sub.frequency(subEncoder.getEncoderValue());
    }
    Serial.println(subEncoder.getEncoderValue());
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
