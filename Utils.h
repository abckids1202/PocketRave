#pragma once

#include <Arduino.h>

namespace Utils {

int16_t clampCoordinate(int16_t value, int16_t minimum, int16_t maximum);
uint8_t approach(uint8_t current, uint8_t target, uint8_t rise, uint8_t fall);
int16_t triangleWave(uint8_t phase, int16_t amplitude);
void debugLine(const __FlashStringHelper* message);

}  // namespace Utils
