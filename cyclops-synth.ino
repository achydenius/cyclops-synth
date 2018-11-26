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
TogglableEncoder subEncoder(15, 16, 2, 100, -5, 5, 0, 1.0);

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
    print(lfoEncoder.getSelectedEncoder() ? "LFO: Amplitude" : "Env: Frequency");
  }

  if (subEncoder.isButtonClicked()) {
    print(subEncoder.getSelectedEncoder() ? "Sub: Amplitude" : "Sub: Detune");
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
    osc.frequency(freq);
    sub.frequency(freq + subEncoder.getEncoderValue(0));
    envelope.noteOn();
    print("* on", freq);
}

void noteOff(byte channel, byte note, byte velocity) {
    envelope.noteOff();
    print("* off");
}

void print(String name) {
  Serial.println(name);
}

void print(String name, float value) {
  Serial.print(name);
  Serial.print(": ");
  Serial.println(value);
}
