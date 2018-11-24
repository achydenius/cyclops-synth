#include "ButtonEncoder.h"

void ButtonEncoder::read() {
    readButton();
    readEncoder();
}

bool ButtonEncoder::hasEncoderValueChanged() {
    return buttonState ?
        second.value != second.previousValue :
        first.value != first.previousValue;
}

float ButtonEncoder::getEncoderValue() {
    return buttonState ? second.mappedValue : first.mappedValue;
}

void ButtonEncoder::setEncoderValues(float firstMapped, float secondMapped) {
    int32_t firstValue = mapValue(firstMapped, first.min, first.max, 0, encoderSteps);
    int32_t secondValue = mapValue(secondMapped, second.min, second.max, 0, encoderSteps);

    first.value = firstValue;
    second.value = secondValue;
    encoder.write(buttonState ? secondValue : firstValue);
}

bool ButtonEncoder::isButtonClicked() {
    return buttonClicked;
}

bool ButtonEncoder::getButtonState() {
    return buttonState;
}

float ButtonEncoder::mapValue(float value, float inMin, float inMax, float outMin, float outMax) {
  return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

void ButtonEncoder::readButton() {
    int buttonValue = digitalRead(buttonPin);
    if (buttonValue == LOW && previousButtonValue == HIGH) {
        buttonClicked = true;
        buttonState = !buttonState;
        setEncoderValues(first.mappedValue, second.mappedValue);
    } else {
        buttonClicked = false;
    }
    previousButtonValue = buttonValue;
}

void ButtonEncoder::readEncoder() {
    int32_t encoderValue = encoder.read();
    if (buttonState) {
        second.previousValue = second.value;
        second.value = encoderValue;
        second.mappedValue = mapValue(second.value, 0, encoderSteps, second.min, second.max);
    } else {
        first.previousValue = first.value;
        first.value = encoderValue;
        first.mappedValue = mapValue(first.value, 0, encoderSteps, first.min, first.max);
    }
}
