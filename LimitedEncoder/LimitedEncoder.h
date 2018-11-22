#ifndef __LIMITED_ENCODER__
#define __LIMITED_ENCODER__

#include <Encoder.h>

class LimitedEncoder : Encoder {
    int32_t min, max;
    int32_t lastAbsolute, lastRelative;
    uint8_t buttonPin;
    int32_t firstValue, secondValue;
    bool firstValueChanged, secondValueChanged;
    bool buttonState;
    bool buttonClicked;
    int previousButtonValue;

public:
    LimitedEncoder(uint8_t encoderPin1, uint8_t encoderPin2, uint8_t buttonPin, int32_t encoderMin, int32_t encoderMax) :
        Encoder(encoderPin1, encoderPin2),
        min(encoderMin),
        max(encoderMax),
        lastAbsolute(0),
        lastRelative(0),
        buttonPin(buttonPin),
        buttonState(false),
        buttonClicked(false),
        previousButtonValue(LOW) {
        pinMode(buttonPin, INPUT_PULLUP);
    }

    int32_t read();
    void write(int32_t value);
    bool hasValueChanged();
    int32_t getValue();
    void update();
    bool isButtonClicked();
    bool getButtonState();

private:
    int32_t clamp(int32_t value, int32_t min, int32_t max);
};

#endif
