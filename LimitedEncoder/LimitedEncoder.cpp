#include "LimitedEncoder.h"

int32_t LimitedEncoder::read() {
    int32_t absolute = Encoder::read();
    int32_t diff = absolute - lastAbsolute;

    if (diff) {
        int32_t relative = clamp(lastRelative + diff, min, max);
        lastRelative = relative;
        lastAbsolute = absolute;
    }

    return lastRelative;
}

void LimitedEncoder::write(int32_t value) {
    int32_t limited = clamp(value, min, max);
    lastRelative = limited;
    lastAbsolute = limited;
    Encoder::write(limited);
}

void LimitedEncoder::update() {
    int buttonValue = digitalRead(buttonPin);
    if (buttonValue == LOW && previousButtonValue == HIGH) {
        buttonClicked = true;
        buttonState = !buttonState;
    } else {
        buttonClicked = false;
    }
    previousButtonValue = buttonValue;

    int32_t encoderValue = read();
    if (!buttonState) {
        if (encoderValue != firstValue) {
            firstValue = encoderValue;
            firstValueChanged = true;
        } else {
            firstValueChanged = false;
        }
    } else {
        if (encoderValue != secondValue) {
            secondValue = encoderValue;
            secondValueChanged = true;
        } else {
            secondValueChanged = false;
        }
    }
}

bool LimitedEncoder::hasValueChanged() {
    return buttonState ? secondValueChanged : firstValueChanged;
}

int32_t LimitedEncoder::getValue() {
    return buttonState ? secondValue : firstValue;
}

bool LimitedEncoder::isButtonClicked() {
    return buttonClicked;
}

bool LimitedEncoder::getButtonState() {
    return buttonState;
}

int32_t LimitedEncoder::clamp(int32_t value, int32_t min, int32_t max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}
