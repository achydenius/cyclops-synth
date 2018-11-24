#ifndef __TOGGLABLE_ENCODER__
#define __TOGGLABLE_ENCODER__

#include "LimitedEncoder.h"

class TogglableEncoder {
    struct EncoderState {
        TogglableEncoder *parent;
        int32_t encoderValue, previousEncoderValue;
        float mappedValue;
        float min, max;

        void setRange(float minValue, float maxValue) {
            min = minValue;
            max = maxValue;
        }

        bool hasChanged() {
            return encoderValue != previousEncoderValue;
        }

        int32_t mappedToEncoder(float value) {
            return (int32_t)mapValue(value, min, max, 0, parent->encoderSteps);
        }

        float encoderToMapped(int32_t value) {
            return mapValue(value, 0, parent->encoderSteps, min, max);
        }

        float mapValue(float value, float inMin, float inMax, float outMin, float outMax) {
            return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
        }
    };

    LimitedEncoder encoderApi;
    int32_t encoderSteps;
    EncoderState encoderStates[2];
    int selectedEncoder;

    uint8_t buttonPin;
    bool buttonClicked;
    int previousButtonValue;

public:
    TogglableEncoder(
        uint8_t encoderPin1, uint8_t encoderPin2, uint8_t buttonPin, int32_t encoderSteps, float firstMin, float firstMax, float secondMin, float secondMax) :
        encoderApi(encoderPin1, encoderPin2, 0, encoderSteps),
        encoderSteps(encoderSteps),
        selectedEncoder(0),
        buttonPin(buttonPin),
        buttonClicked(false),
        previousButtonValue(LOW)
    {
        pinMode(buttonPin, INPUT_PULLUP);
        encoderStates[0].parent = this;
        encoderStates[1].parent = this;
        encoderStates[0].setRange(firstMin, firstMax);
        encoderStates[1].setRange(secondMin, secondMax);
    }

    void read();
    int getSelectedEncoder();
    bool hasEncoderValueChanged();
    float getEncoderValue();
    void setEncoderValues(float first, float second);
    bool isButtonClicked();

private:
    void readButton();
    void readEncoder();
};

#endif
