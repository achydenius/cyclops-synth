#ifndef __LIMITED_ENCODER__
#define __LIMITED_ENCODER__

#include <Encoder.h>

class LimitedEncoder : Encoder {
    int32_t min, max;
    int32_t lastAbsolute, lastRelative;

public:
    LimitedEncoder(uint8_t pin1, uint8_t pin2, int32_t min, int32_t max) :
        Encoder(pin1, pin2),
        min(min),
        max(max),
        lastAbsolute(0),
        lastRelative(0) {}

    int32_t read();
    void write(int32_t value);

private:
    int32_t clamp(int32_t value, int32_t min, int32_t max);
};

#endif
