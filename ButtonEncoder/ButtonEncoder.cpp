#include "ButtonEncoder.h"

void ButtonEncoder::update() {
    int buttonValue = digitalRead(buttonPin);
    if (buttonValue == LOW && previousButtonValue == HIGH) {
        buttonClicked = true;
        buttonState = !buttonState;
    } else {
        buttonClicked = false;
    }
    previousButtonValue = buttonValue;

    int32_t encoderValue = encoder.read();
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

bool ButtonEncoder::hasValueChanged() {
    return buttonState ? secondValueChanged : firstValueChanged;
}

int32_t ButtonEncoder::getValue() {
    return buttonState ? secondValue : firstValue;
}

void ButtonEncoder::setValue(int32_t value) {
    encoder.write(value);
}

bool ButtonEncoder::isButtonClicked() {
    return buttonClicked;
}

bool ButtonEncoder::getButtonState() {
    return buttonState;
}

