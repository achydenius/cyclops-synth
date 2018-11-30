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
AudioConnection          patchCord7(envelope, 0, usb, 1);
// GUItool: end automatically generated code

const int envEncoderPin1 = 19;
const int envEncoderPin2 = 20;
const int envButtonPin = 0;
const int lfoEncoderPin1 = 17;
const int lfoEncoderPin2 = 18;
const int lfoButtonPin = 1;
const int subEncoderPin1 = 15;
const int subEncoderPin2 = 16;
const int subButtonPin = 2;

TogglableEncoder envEncoder(envEncoderPin1, envEncoderPin2, envButtonPin, 100, 0, 5000, 0, 5000);
TogglableEncoder lfoEncoder(lfoEncoderPin1, lfoEncoderPin2, lfoButtonPin, 100, 0, 10.0, 0, 0.95);
TogglableEncoder subEncoder(subEncoderPin1, subEncoderPin2, subButtonPin, 50, 1.5, 2.5 , 0, 1.0);

void setup() {
  Serial.begin(9600);

  AudioMemory(10);

  osc.frequency(440);
  osc.amplitude(1.0);

  envEncoder.setEncoderValues(50, 500);
  lfoEncoder.setEncoderValues(2.5, 30);
  subEncoder.setEncoderValues(1.5, 0);

  envelope.attack(envEncoder.getEncoderValue(0));
  envelope.decay(0);
  envelope.sustain(1.0);
  envelope.release(envEncoder.getEncoderValue(1));

  lfo.frequency(lfoEncoder.getEncoderValue(0));
  lfo.amplitude(lfoEncoder.getEncoderValue(1));

  sub.frequency(subEncoder.getEncoderValue(0));
  sub.amplitude(subEncoder.getEncoderValue(1));

  usbMIDI.setHandleNoteOn(noteOn);
  usbMIDI.setHandleNoteOff(noteOff);
}

void loop() {
  envEncoder.read();
  lfoEncoder.read();
  subEncoder.read();

  if (envEncoder.isButtonClicked()) {
    print(envEncoder.getSelectedEncoder() ? "Env: Release" : "Env: Attack");
  }

  if (lfoEncoder.isButtonClicked()) {
    print(lfoEncoder.getSelectedEncoder() ? "LFO: Amplitude" : "LFO: Frequency");
  }

  if (subEncoder.isButtonClicked()) {
    print(subEncoder.getSelectedEncoder() ? "Sub: Amplitude" : "Sub: Frequency");
  }

  if (envEncoder.hasEncoderValueChanged()) {
    float value = envEncoder.getEncoderValue();
    if (envEncoder.getSelectedEncoder()) {
      envelope.release(value);
      print("Env/Attack", value);
    } else {
      envelope.attack(value);
      print("Env/Release", value);
    }
  }

  if (lfoEncoder.hasEncoderValueChanged()) {
    float value = lfoEncoder.getEncoderValue();
    if (lfoEncoder.getSelectedEncoder()) {
      lfo.amplitude(value);
      print("LFO/Amp", value);
    } else {
      lfo.frequency(value);
      print("LFO/Freq", value);
    }
  }
  
  if (subEncoder.hasEncoderValueChanged()) {
    float value = subEncoder.getEncoderValue();
    if (subEncoder.getSelectedEncoder()) {
      sub.amplitude(value);
      print("Sub/Amp", value);
    } else {
      sub.frequency(value);
      print("Sub/Freq", value);
    }
  }

  usbMIDI.read();
}

void noteOn(byte channel, byte note, byte velocity) {
    float freq = pow(2.0, (note - 69) / 12.0) * 440.0;
    float subFreq  = freq / pow(2, int(subEncoder.getEncoderValue(0)));
    osc.frequency(freq);
    sub.frequency(subFreq);
    envelope.noteOn();
}

void noteOff(byte channel, byte note, byte velocity) {
    envelope.noteOff();
}

void print(String name) {
  Serial.println(name);
}

void print(String name, float value) {
  Serial.print(name);
  Serial.print(": ");
  Serial.println(value);
}
