#pragma once

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

enum class AnimationMode : uint8_t {
  Equalizer,
  MirroredEqualizer,
  Pulse,
  Wave,
  Starfield,
  Tunnel,
  Particles,
  GlitchText,
  VisualPulse,
  Count
};

namespace Animations {

void initialize(AnimationMode mode);
void update(AnimationMode mode, unsigned long now);
void render(AnimationMode mode, Adafruit_SSD1306& display);
const __FlashStringHelper* modeName(AnimationMode mode);

}  // namespace Animations
