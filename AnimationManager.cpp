#include "AnimationManager.h"

#include "Animations.h"
#include "Config.h"
#include "DisplayManager.h"

namespace {

enum class TransitionState : uint8_t {
  None,
  TransitionOut,
  TransitionIn
};

AnimationMode currentMode = AnimationMode::Equalizer;
AnimationMode nextMode = AnimationMode::Pulse;
TransitionState transitionState = TransitionState::None;
unsigned long modeStartedAt = 0;
unsigned long currentModeDuration = Config::MODE_DURATION_MIN_MS;
unsigned long transitionStartedAt = 0;

AnimationMode randomModeExcept(AnimationMode excluded) {
  uint8_t modeValue;
  do {
    modeValue = static_cast<uint8_t>(
        random(0, static_cast<uint8_t>(AnimationMode::Count)));
  } while (modeValue == static_cast<uint8_t>(excluded) ||
           (!Config::ENABLE_VISUAL_PULSE &&
            modeValue == static_cast<uint8_t>(AnimationMode::VisualPulse)));
  return static_cast<AnimationMode>(modeValue);
}

void printModeDebug() {
  if (!Config::DEBUG_MODE) {
    return;
  }
  Serial.print(F("Mode: "));
  Serial.print(Animations::modeName(currentMode));
  Serial.print(F(" | duration: "));
  Serial.print(currentModeDuration);
  Serial.println(F(" ms"));
}

void beginTransition(unsigned long now) {
  nextMode = randomModeExcept(currentMode);
  transitionState = TransitionState::TransitionOut;
  transitionStartedAt = now;
}

void drawTransitionMask(unsigned long now) {
  Adafruit_SSD1306& oled = DisplayManager::display();
  const uint32_t elapsed = now - transitionStartedAt;
  const uint32_t calculatedProgress =
      (elapsed * 128UL) / Config::TRANSITION_DURATION_MS;
  const uint8_t progress = static_cast<uint8_t>(
      calculatedProgress < 128UL ? calculatedProgress : 128UL);

  if (transitionState == TransitionState::TransitionOut) {
    oled.fillRect(0, 0, progress, Config::SCREEN_HEIGHT, SSD1306_BLACK);
    for (uint8_t x = progress % 4; x < progress; x += 4) {
      oled.drawFastVLine(x, 0, Config::SCREEN_HEIGHT, SSD1306_WHITE);
    }
  } else if (transitionState == TransitionState::TransitionIn) {
    const uint8_t remaining = 128 - progress;
    oled.fillRect(0, 0, remaining, Config::SCREEN_HEIGHT, SSD1306_BLACK);
  }
}

}  // namespace

namespace AnimationManager {

void begin(unsigned long now) {
  currentMode = AnimationMode::Equalizer;
  currentModeDuration = static_cast<unsigned long>(
      random(Config::MODE_DURATION_MIN_MS, Config::MODE_DURATION_MAX_MS + 1));
  modeStartedAt = now;
  transitionState = TransitionState::None;
  Animations::initialize(currentMode);
  printModeDebug();
}

void update(unsigned long now) {
  if (transitionState == TransitionState::None &&
      now - modeStartedAt >= currentModeDuration) {
    beginTransition(now);
  }

  if (transitionState == TransitionState::TransitionOut &&
      now - transitionStartedAt >= Config::TRANSITION_DURATION_MS) {
    currentMode = nextMode;
    Animations::initialize(currentMode);
    transitionState = TransitionState::TransitionIn;
    transitionStartedAt = now;
  } else if (transitionState == TransitionState::TransitionIn &&
             now - transitionStartedAt >= Config::TRANSITION_DURATION_MS) {
    transitionState = TransitionState::None;
    modeStartedAt = now;
    currentModeDuration = static_cast<unsigned long>(
        random(Config::MODE_DURATION_MIN_MS, Config::MODE_DURATION_MAX_MS + 1));
    printModeDebug();
  }

  Animations::update(currentMode, now);
}

void render(unsigned long now) {
  Animations::render(currentMode, DisplayManager::display());
  if (transitionState != TransitionState::None) {
    drawTransitionMask(now);
  }
}

}  // namespace AnimationManager
