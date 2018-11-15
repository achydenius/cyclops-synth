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

const int encoderSteps = 100;

LimitedEncoder attackEncoder(20, 19, 0, encoderSteps);
LimitedEncoder releaseEncoder(18, 17, 0, encoderSteps);
LimitedEncoder phaseEncoder(16, 15, 0, encoderSteps);
const int modePin = 0;

float attackValue = 0;
float releaseValue = 0;
float phaseValue = 0;
bool buttonValue = HIGH;
bool lfoMode = false;

void setup() {
  pinMode(modePin, INPUT_PULLUP);

  Serial.begin(9600);

  AudioMemory(10);
  pwm.frequency(440);
  pwm.amplitude(0.5);
  dc.amplitude(0.5);

  attackEncoder.write(1);
  releaseEncoder.write(5);
  phaseEncoder.write(50);

  envelope.decay(0);
  envelope.sustain(1.0);
}

void loop() {
  float attack = map_float(attackEncoder.read(), 0, encoderSteps, 0, 10000);
  float release = map_float(releaseEncoder.read(), 0, encoderSteps, 0, 10000);
  float phase = map_float(phaseEncoder.read(), 0, encoderSteps, -0.95, 0.95);
  bool button = digitalRead(modePin);

  if (attack != attackValue) {
    envelope.attack(attack);
    attackValue = attack;
  }

  if (release != releaseValue) {
    envelope.release(release);
    releaseValue = release;
  }

  if (phase != phaseValue) {
    dc.amplitude(phase);
    phaseValue = phase;
  }

  if (button == LOW && button != buttonValue) {
    lfoMode = !lfoMode;
    Serial.println(lfoMode);
  }
  buttonValue = button;
  
  envelope.noteOn();
  delay(100);
  envelope.noteOff();
  delay(1000);
}

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
