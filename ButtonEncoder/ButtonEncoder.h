#ifndef __BUTTON_ENCODER__
#define __BUTTON_ENCODER__

#include "LimitedEncoder.h"

class ButtonEncoder {
    LimitedEncoder encoder;
    int32_t firstValue, secondValue;
    bool firstValueChanged, secondValueChanged;

    uint8_t buttonPin;
    bool buttonState;
    bool buttonClicked;
    int previousButtonValue;

public:
    ButtonEncoder(uint8_t encoderPin1, uint8_t encoderPin2, int32_t encoderMin, int32_t encoderMax, uint8_t buttonPin) :
        encoder(encoderPin1, encoderPin2, encoderMin, encoderMax),
        buttonPin(buttonPin),
        buttonState(false),
        buttonClicked(false),
        previousButtonValue(LOW)
    {
        pinMode(buttonPin, INPUT_PULLUP);
    }

    bool hasValueChanged();
    int32_t getValue();
    void setValue(int32_t value);
    void update();
    bool isButtonClicked();
    bool getButtonState();
};

#endif
