#ifndef __BUTTON_ENCODER__
#define __BUTTON_ENCODER__

#include "LimitedEncoder.h"

class ButtonEncoder {
    typedef struct EncoderParameters {
        int32_t value;
        int32_t previousValue;
        float mappedValue;
        float min;
        float max;

        EncoderParameters(float min, float max) : min(min), max(max) {}
    } EncoderParameters;

    LimitedEncoder encoder;
    int encoderSteps;
    EncoderParameters first, second;

    uint8_t buttonPin;
    bool buttonState;
    bool buttonClicked;
    int previousButtonValue;

public:
    ButtonEncoder(
        uint8_t encoderPin1, uint8_t encoderPin2, uint8_t buttonPin, int32_t encoderSteps, float firstMin, float firstMax, float secondMin, float secondMax) :
        encoder(encoderPin1, encoderPin2, 0, encoderSteps),
        encoderSteps(encoderSteps),
        first(firstMin, firstMax),
        second(secondMin, secondMax),
        buttonPin(buttonPin),
        buttonState(false),
        buttonClicked(false),
        previousButtonValue(LOW)
    {
        pinMode(buttonPin, INPUT_PULLUP);
    }

    void read();
    bool hasEncoderValueChanged();
    float getEncoderValue();
    void setEncoderValues(float first, float second);
    bool isButtonClicked();
    bool getButtonState();

private:
    float mapValue(float value, float inMin, float inMax, float outMin, float outMax);
    void readButton();
    void readEncoder();
};

#endif
