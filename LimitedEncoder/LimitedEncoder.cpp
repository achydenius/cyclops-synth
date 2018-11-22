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
    int value = digitalRead(buttonPin);
    if (value == LOW && previousButtonValue == HIGH) {
        buttonClicked = true;
        buttonState = !buttonState;
    } else {
        buttonClicked = false;
    }
    previousButtonValue = value;
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
