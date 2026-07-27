#pragma once

#include <Adafruit_SSD1306.h>

namespace DisplayManager {

bool begin();
Adafruit_SSD1306& display();
void clear();
void present();
void setInverted(bool inverted);
void renderStartup(unsigned long elapsedMs);

}  // namespace DisplayManager
