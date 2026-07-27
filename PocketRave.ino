#include <Arduino.h>

#include "AnimationManager.h"
#include "Config.h"
#include "DisplayManager.h"
#include "Utils.h"

enum class AppState : uint8_t {
  Startup,
  Running
};

static AppState appState = AppState::Startup;
static unsigned long startupStartedAt = 0;
static unsigned long lastFrameAt = 0;
static unsigned long fpsWindowStartedAt = 0;
static uint16_t framesInWindow = 0;

static void reportFps(unsigned long now) {
  if (!Config::DEBUG_MODE ||
      now - fpsWindowStartedAt < Config::FPS_REPORT_INTERVAL_MS) {
    return;
  }

  const unsigned long elapsed = now - fpsWindowStartedAt;
  const uint16_t fpsTimesTen =
      static_cast<uint16_t>((framesInWindow * 10000UL) / elapsed);
  Serial.print(F("FPS: "));
  Serial.print(fpsTimesTen / 10);
  Serial.print('.');
  Serial.println(fpsTimesTen % 10);
  fpsWindowStartedAt = now;
  framesInWindow = 0;
}

void setup() {
  Serial.begin(Config::SERIAL_BAUD);
  Utils::debugLine(F("Pocket Rave starting"));

  randomSeed(analogRead(A0) ^ micros());

  if (!DisplayManager::begin()) {
    Serial.println(F("SSD1306 initialization failed"));
    Serial.println(F("Check OLED address, wiring, voltage, and controller."));
    while (true) {
      delay(100);
    }
  }

  Utils::debugLine(F("Display initialized"));
  startupStartedAt = millis();
  lastFrameAt = startupStartedAt;
  fpsWindowStartedAt = startupStartedAt;
}

void loop() {
  const unsigned long now = millis();

  if (now - lastFrameAt < Config::FRAME_INTERVAL_MS) {
    return;
  }

  // Advance by a fixed interval so occasional slow frames do not cause drift.
  lastFrameAt += Config::FRAME_INTERVAL_MS;
  if (now - lastFrameAt > Config::FRAME_INTERVAL_MS * 3UL) {
    lastFrameAt = now;
  }

  if (appState == AppState::Startup) {
    const unsigned long elapsed = now - startupStartedAt;
    DisplayManager::renderStartup(elapsed);
    if (elapsed >= Config::STARTUP_DURATION_MS) {
      appState = AppState::Running;
      AnimationManager::begin(now);
    }
    return;
  }

  AnimationManager::update(now);
  DisplayManager::clear();
  AnimationManager::render(now);
  DisplayManager::present();

  ++framesInWindow;
  reportFps(now);
}
