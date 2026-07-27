#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

constexpr int16_t SCREEN_WIDTH = 128;
constexpr int16_t SCREEN_HEIGHT = 64;
constexpr int8_t OLED_RESET_PIN = -1;
constexpr uint8_t OLED_ADDRESS = 0x3C;

Adafruit_SSD1306 display(
    SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 initialization failed"));
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(27, 5);
  display.println(F("POCKET RAVE"));
  display.drawPixel(4, 4, SSD1306_WHITE);
  display.drawFastHLine(4, 15, 120, SSD1306_WHITE);
  display.drawFastVLine(4, 15, 45, SSD1306_WHITE);
  display.drawRect(14, 22, 28, 20, SSD1306_WHITE);
  display.drawCircle(64, 32, 12, SSD1306_WHITE);
  display.fillRect(91, 22, 24, 20, SSD1306_WHITE);
  display.display();
  Serial.println(F("OLED graphics test displayed"));
}

void loop() {
  // Static test image.
}
