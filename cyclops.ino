#include <LimitedEncoder.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioOutputI2S           i2s;           //xy=205,346
AudioSynthWaveformDc     dc;            //xy=289,198
AudioSynthWaveformPWM    pwm;           //xy=436,373
AudioEffectEnvelope      envelope;      //xy=652,286
AudioOutputUSB           usb;           //xy=897,408
AudioConnection          patchCord1(dc, pwm);
AudioConnection          patchCord2(pwm, envelope);
AudioConnection          patchCord3(envelope, 0, usb, 0);
AudioConnection          patchCord4(envelope, 0, usb, 1);
// GUItool: end automatically generated code

LimitedEncoder attackEncoder(20, 19, -100, 100);
LimitedEncoder releaseEncoder(18, 17, -100, 100);

float attackValue = 0;
float releaseValue = 250;

void setup() {
  Serial.begin(9600);

  AudioMemory(10);
  pwm.frequency(440);
  pwm.amplitude(0.5);
  dc.amplitude(0.5);
  
  envelope.attack(attackValue);
  envelope.decay(0);
  envelope.sustain(1.0);
  envelope.release(releaseValue);
}

void loop() {
  float attack = map_float(attackEncoder.read(), -100, 100, 0, 10000);
  float release = map_float(releaseEncoder.read(), -100, 100, 0, 10000);

  if (attack != attackValue) {
    envelope.attack(attack);
    attackValue = attack;
  }

  if (release != releaseValue) {
    envelope.release(release);
    releaseValue = release;
  }

  envelope.noteOn();
  delay(100);
  envelope.noteOff();
  delay(1000);
}

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
