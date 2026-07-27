#pragma once

#include <Arduino.h>

namespace AnimationManager {

void begin(unsigned long now);
void update(unsigned long now);
void render(unsigned long now);

}  // namespace AnimationManager
