#include "DisplayManager.h"

#include <Adafruit_GFX.h>
#include <Wire.h>

#include "Config.h"

namespace {

Adafruit_SSD1306 oled(
    Config::SCREEN_WIDTH,
    Config::SCREEN_HEIGHT,
    &Wire,
    Config::OLED_RESET_PIN);

}  // namespace

namespace DisplayManager {

bool begin() {
  Wire.begin();
  if (!oled.begin(SSD1306_SWITCHCAPVCC, Config::OLED_ADDRESS)) {
    return false;
  }

  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextWrap(false);
  oled.display();
  return true;
}

Adafruit_SSD1306& display() {
  return oled;
}

void clear() {
  oled.clearDisplay();
}

void present() {
  oled.display();
}

void setInverted(bool inverted) {
  oled.invertDisplay(inverted);
}

void renderStartup(unsigned long elapsedMs) {
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);

  if (elapsedMs < 650) {
    oled.setTextSize(1);
    oled.setCursor(43, 27);
    oled.print(F("WAKE"));
    const uint8_t x = static_cast<uint8_t>((elapsedMs / 45UL) % 128UL);
    oled.drawFastVLine(x, 0, Config::SCREEN_HEIGHT, SSD1306_WHITE);
  } else if (elapsedMs < 1500) {
    oled.setTextSize(2);
    oled.setCursor(27, 12);
    oled.print(F("POCKET"));
    oled.setCursor(39, 36);
    oled.print(F("RAVE"));
  } else {
    oled.setTextSize(1);
    oled.setCursor(31, 17);
    oled.print(F("SYSTEM READY"));
    oled.drawRect(15, 38, 98, 8, SSD1306_WHITE);
    const uint16_t progress = static_cast<uint16_t>(elapsedMs - 1500UL);
    const uint16_t calculatedWidth =
        static_cast<uint16_t>((progress * 94UL) / 1100UL);
    const uint8_t width =
        static_cast<uint8_t>(calculatedWidth < 94 ? calculatedWidth : 94);
    oled.fillRect(17, 40, width, 4, SSD1306_WHITE);
  }

  oled.display();
}

}  // namespace DisplayManager
