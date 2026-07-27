#pragma once

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

namespace ParticleSystem {

void initialize();
void update();
void render(Adafruit_SSD1306& display);

}  // namespace ParticleSystem
