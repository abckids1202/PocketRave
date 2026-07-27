#include "Utils.h"

#include "Config.h"

namespace Utils {

int16_t clampCoordinate(int16_t value, int16_t minimum, int16_t maximum) {
  if (value < minimum) {
    return minimum;
  }
  if (value > maximum) {
    return maximum;
  }
  return value;
}

uint8_t approach(uint8_t current, uint8_t target, uint8_t rise, uint8_t fall) {
  if (current < target) {
    const uint16_t next = static_cast<uint16_t>(current) + rise;
    return static_cast<uint8_t>(next < target ? next : target);
  }
  if (current > target) {
    return current > fall && current - fall > target ? current - fall : target;
  }
  return current;
}

int16_t triangleWave(uint8_t phase, int16_t amplitude) {
  const uint8_t ramp = phase < 128 ? phase : 255 - phase;
  return static_cast<int16_t>(
      (static_cast<int32_t>(ramp) * amplitude * 2L) / 127L - amplitude);
}

void debugLine(const __FlashStringHelper* message) {
  if (Config::DEBUG_MODE) {
    Serial.println(message);
  }
}

}  // namespace Utils
