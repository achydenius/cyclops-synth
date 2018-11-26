#include "TogglableEncoder.h"

void TogglableEncoder::read() {
    readButton();
    readEncoder();
}

int TogglableEncoder::getSelectedEncoder() {
    return selectedEncoder;
}

bool TogglableEncoder::hasEncoderValueChanged() {
    return encoderStates[selectedEncoder].hasChanged();
}

float TogglableEncoder::getEncoderValue() {
    return getEncoderValue(selectedEncoder);
}

float TogglableEncoder::getEncoderValue(int encoder) {
    return encoderStates[encoder].mappedValue;
}

void TogglableEncoder::setEncoderValues(float firstMapped, float secondMapped) {
    encoderStates[0].encoderValue = encoderStates[0].mappedToEncoder(firstMapped);
    encoderStates[1].encoderValue = encoderStates[1].mappedToEncoder(secondMapped);
    encoderApi.write(encoderStates[selectedEncoder].encoderValue);
}

bool TogglableEncoder::isButtonClicked() {
    return buttonApi.fallingEdge();
}

void TogglableEncoder::readButton() {
    buttonApi.update();
    if (buttonApi.fallingEdge()) {
        selectedEncoder = !selectedEncoder;
        setEncoderValues(encoderStates[0].mappedValue, encoderStates[1].mappedValue);
    }
}

void TogglableEncoder::readEncoder() {
    int32_t value = encoderApi.read();

    EncoderState *state = &encoderStates[selectedEncoder];
    state->previousEncoderValue = state->encoderValue;
    state->encoderValue = value;
    state->mappedValue = state->encoderToMapped(value);
}
