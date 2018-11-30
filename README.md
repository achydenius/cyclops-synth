# Cyclops

A simple monophonic PWM synth that runs on [Teensy 3.2](https://www.pjrc.com/store/teensy32.html).

The synth has following features:

* MIDI input via USB (only note on/off is supported)
* Audio output via USB
* Timbre controls
  * Envelope with attack and release parameters that controls the main amplitude
  * LFO with frequency and amplitude parameters that controls the PWM
  * Sub oscillator with frequency (one or two octaves down) and amplitude parameters

![Photo](https://raw.githubusercontent.com/achydenius/cyclops-synth/master/photo.jpg "Photo")

## Implementation notes

The synth reads input from three rotary encoders with buttons. Pins for these can be configured with `const` variables in the [cyclops-synth.ino](./cyclops-synth.ino).

Rotary encoder input uses `Encoder` class from Teensyduino library as a basis. As this project needed some additional logic, classes `LimitedEncoder` and `TogglableEncoder` were created. The former limits encoder's values to a range and the latter adds an ability to control two values with a single encoder and choose between them by pressing a button. Both of the classes should be easy to use in other projects as well.

## External dependencies

[Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) needs to be installed and the following libraries have been used:

* [Audio](https://www.pjrc.com/teensy/td_libs_Audio.html) for generating the sounds
* [USB MIDI](https://www.pjrc.com/teensy/td_midi.html) for reading MIDI messages via USB
* [Encoder](https://www.pjrc.com/teensy/td_libs_Encoder.html) for low-level encoder input
* [Bounce](https://www.pjrc.com/teensy/td_libs_Bounce.html) for debouncing buttons
