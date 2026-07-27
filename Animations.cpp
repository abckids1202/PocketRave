#include "Animations.h"

#include <Adafruit_GFX.h>

#include "Config.h"
#include "ParticleSystem.h"
#include "Utils.h"

namespace {

uint8_t barHeights[Config::EQUALIZER_BAR_COUNT];
uint8_t barTargets[Config::EQUALIZER_BAR_COUNT];
uint8_t peakHeights[Config::EQUALIZER_BAR_COUNT];
uint8_t equalizerTick = 0;

uint8_t pulseRadius = 0;
uint8_t pulseOffset = 0;

uint8_t wavePhase = 0;
uint8_t waveAmplitude = 13;

struct Star {
  int8_t x;
  int8_t y;
  uint8_t depth;
};
Star stars[Config::STAR_COUNT];

uint8_t tunnelPhase = 0;
int8_t tunnelCenterX = Config::SCREEN_WIDTH / 2;
int8_t tunnelCenterY = Config::SCREEN_HEIGHT / 2;

uint8_t glitchTextIndex = 0;
uint8_t glitchStrength = 1;
uint8_t visualPulsePhase = 0;

void resetStar(Star& star, bool randomDepth) {
  star.x = static_cast<int8_t>(random(-58, 59));
  star.y = static_cast<int8_t>(random(-29, 30));
  if (star.x > -5 && star.x < 5) {
    star.x = star.x < 0 ? -8 : 8;
  }
  star.depth = randomDepth ? static_cast<uint8_t>(random(16, 64)) : 63;
}

void initializeEqualizer() {
  equalizerTick = 0;
  for (uint8_t index = 0; index < Config::EQUALIZER_BAR_COUNT; ++index) {
    barHeights[index] = static_cast<uint8_t>(random(3, 28));
    barTargets[index] = static_cast<uint8_t>(random(4, 31));
    peakHeights[index] = barHeights[index];
  }
}

void updateEqualizer(bool mirrored) {
  ++equalizerTick;
  if ((equalizerTick % 7) == 0) {
    for (uint8_t index = 0; index < Config::EQUALIZER_BAR_COUNT; ++index) {
      barTargets[index] = static_cast<uint8_t>(
          random(3, mirrored ? 29 : 57));
    }
  }

  for (uint8_t index = 0; index < Config::EQUALIZER_BAR_COUNT; ++index) {
    barHeights[index] =
        Utils::approach(barHeights[index], barTargets[index], 3, 1);
    if (barHeights[index] >= peakHeights[index]) {
      peakHeights[index] = barHeights[index];
    } else if ((equalizerTick % 3) == 0 && peakHeights[index] > 1) {
      --peakHeights[index];
    }
  }
}

void renderEqualizer(Adafruit_SSD1306& display, bool mirrored) {
  constexpr uint8_t barSlot = Config::SCREEN_WIDTH /
                              Config::EQUALIZER_BAR_COUNT;
  for (uint8_t index = 0; index < Config::EQUALIZER_BAR_COUNT; ++index) {
    const int16_t x = index * barSlot;
    if (mirrored) {
      const uint8_t height =
          barHeights[index] < 29 ? barHeights[index] : 29;
      display.fillRect(x, 32 - height, barSlot - 2, height, SSD1306_WHITE);
      display.fillRect(x, 33, barSlot - 2, height, SSD1306_WHITE);
    } else {
      const uint8_t height =
          barHeights[index] < 57 ? barHeights[index] : 57;
      display.fillRect(
          x, Config::SCREEN_HEIGHT - height, barSlot - 2, height, SSD1306_WHITE);
      const uint8_t peak =
          peakHeights[index] < 62 ? peakHeights[index] : 62;
      display.drawFastHLine(
          x, Config::SCREEN_HEIGHT - 1 - peak, barSlot - 2, SSD1306_WHITE);
    }
  }
}

void renderPulse(Adafruit_SSD1306& display) {
  const int16_t centerX = Config::SCREEN_WIDTH / 2 +
                          Utils::triangleWave(pulseOffset, 8);
  const int16_t centerY = Config::SCREEN_HEIGHT / 2 +
                          Utils::triangleWave(pulseOffset + 64, 4);
  for (uint8_t layer = 0; layer < 4; ++layer) {
    const uint8_t radius = static_cast<uint8_t>((pulseRadius + layer * 12) % 48);
    if (radius > 1) {
      display.drawCircle(centerX, centerY, radius, SSD1306_WHITE);
    }
  }
  display.fillCircle(centerX, centerY, 2, SSD1306_WHITE);
}

void renderWave(Adafruit_SSD1306& display) {
  int16_t previousY1 = Config::SCREEN_HEIGHT / 2;
  int16_t previousY2 = Config::SCREEN_HEIGHT / 2;
  for (int16_t x = 1; x < Config::SCREEN_WIDTH; ++x) {
    const uint8_t phase1 = static_cast<uint8_t>(x * 5 + wavePhase);
    const uint8_t phase2 = static_cast<uint8_t>(x * 3 - wavePhase * 2);
    const int16_t y1 = Config::SCREEN_HEIGHT / 2 +
                       Utils::triangleWave(phase1, waveAmplitude);
    const int16_t y2 = Config::SCREEN_HEIGHT / 2 +
                       Utils::triangleWave(phase2, waveAmplitude / 2);
    display.drawLine(x - 1, previousY1, x, y1, SSD1306_WHITE);
    if ((x & 1) == 0) {
      display.drawPixel(x, y2, SSD1306_WHITE);
    }
    previousY1 = y1;
    previousY2 = y2;
  }
  (void)previousY2;
}

void renderStarfield(Adafruit_SSD1306& display) {
  for (uint8_t index = 0; index < Config::STAR_COUNT; ++index) {
    const int16_t x = Config::SCREEN_WIDTH / 2 +
                      (static_cast<int16_t>(stars[index].x) * 32) /
                          stars[index].depth;
    const int16_t y = Config::SCREEN_HEIGHT / 2 +
                      (static_cast<int16_t>(stars[index].y) * 32) /
                          stars[index].depth;
    if (x >= 0 && x < Config::SCREEN_WIDTH &&
        y >= 0 && y < Config::SCREEN_HEIGHT) {
      display.drawPixel(x, y, SSD1306_WHITE);
      if (stars[index].depth < 20 && x + 1 < Config::SCREEN_WIDTH) {
        display.drawPixel(x + 1, y, SSD1306_WHITE);
      }
    }
  }
  display.drawPixel(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2,
                    SSD1306_WHITE);
}

void renderTunnel(Adafruit_SSD1306& display) {
  for (uint8_t layer = 0; layer < 6; ++layer) {
    const uint8_t size = static_cast<uint8_t>((tunnelPhase + layer * 11) % 66);
    if (size < 3) {
      continue;
    }
    const int16_t halfWidth = size;
    const int16_t halfHeight = size / 2;
    display.drawRect(tunnelCenterX - halfWidth,
                     tunnelCenterY - halfHeight,
                     halfWidth * 2,
                     halfHeight * 2,
                     SSD1306_WHITE);
  }
  display.drawLine(0, 0, tunnelCenterX, tunnelCenterY, SSD1306_WHITE);
  display.drawLine(Config::SCREEN_WIDTH - 1, 0,
                   tunnelCenterX, tunnelCenterY, SSD1306_WHITE);
  display.drawLine(0, Config::SCREEN_HEIGHT - 1,
                   tunnelCenterX, tunnelCenterY, SSD1306_WHITE);
  display.drawLine(Config::SCREEN_WIDTH - 1, Config::SCREEN_HEIGHT - 1,
                   tunnelCenterX, tunnelCenterY, SSD1306_WHITE);
}

void renderGlitchText(Adafruit_SSD1306& display) {
  static const char* const labels[] = {
      "RAVE", "DROP", "MOVE", "NO SIGNAL", "404 REALITY"};
  constexpr uint8_t labelCount = sizeof(labels) / sizeof(labels[0]);
  const char* text = labels[glitchTextIndex % labelCount];
  const uint8_t textSize = glitchTextIndex < 3 ? 2 : 1;
  const int16_t baseX = textSize == 2 ? 34 : 29;
  const int16_t baseY = textSize == 2 ? 25 : 28;

  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(baseX + random(-glitchStrength, glitchStrength + 1),
                    baseY + random(-1, 2));
  display.print(text);

  for (uint8_t line = 0; line < glitchStrength + 1; ++line) {
    const int16_t y = random(4, Config::SCREEN_HEIGHT - 4);
    display.drawFastHLine(random(0, 30), y, random(20, 90), SSD1306_WHITE);
  }
}

void renderVisualPulse(Adafruit_SSD1306& display) {
  const uint8_t beat = visualPulsePhase < 128
                           ? visualPulsePhase
                           : static_cast<uint8_t>(255 - visualPulsePhase);
  const uint8_t inset = static_cast<uint8_t>((beat * 26UL) / 127UL);
  for (uint8_t ring = 0; ring < 3; ++ring) {
    const int16_t offset = inset + ring * 7;
    const int16_t width = Config::SCREEN_WIDTH - offset * 2;
    const int16_t height = Config::SCREEN_HEIGHT - offset;
    if (width > 2 && height > 2) {
      display.drawRect(offset, offset / 2, width, height, SSD1306_WHITE);
    }
  }
  if (beat > 98) {
    display.fillCircle(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2,
                       3, SSD1306_WHITE);
  }
}

}  // namespace

namespace Animations {

void initialize(AnimationMode mode) {
  switch (mode) {
    case AnimationMode::Equalizer:
    case AnimationMode::MirroredEqualizer:
      initializeEqualizer();
      break;
    case AnimationMode::Pulse:
      pulseRadius = 0;
      pulseOffset = static_cast<uint8_t>(random(0, 255));
      break;
    case AnimationMode::Wave:
      wavePhase = 0;
      waveAmplitude = static_cast<uint8_t>(random(10, 19));
      break;
    case AnimationMode::Starfield:
      for (uint8_t index = 0; index < Config::STAR_COUNT; ++index) {
        resetStar(stars[index], true);
      }
      break;
    case AnimationMode::Tunnel:
      tunnelPhase = 0;
      tunnelCenterX = static_cast<int8_t>(random(54, 75));
      tunnelCenterY = static_cast<int8_t>(random(27, 38));
      break;
    case AnimationMode::Particles:
      ParticleSystem::initialize();
      break;
    case AnimationMode::GlitchText:
      glitchTextIndex = static_cast<uint8_t>(random(0, 5));
      glitchStrength = 1;
      break;
    case AnimationMode::VisualPulse:
      visualPulsePhase = 0;
      break;
    case AnimationMode::Count:
      break;
  }
}

void update(AnimationMode mode, unsigned long now) {
  switch (mode) {
    case AnimationMode::Equalizer:
      updateEqualizer(false);
      break;
    case AnimationMode::MirroredEqualizer:
      updateEqualizer(true);
      break;
    case AnimationMode::Pulse:
      pulseRadius = static_cast<uint8_t>((pulseRadius + 2) % 48);
      pulseOffset += 2;
      break;
    case AnimationMode::Wave:
      wavePhase += 4;
      break;
    case AnimationMode::Starfield:
      for (uint8_t index = 0; index < Config::STAR_COUNT; ++index) {
        if (stars[index].depth <= 3) {
          resetStar(stars[index], false);
        } else {
          stars[index].depth -= stars[index].depth > 30 ? 2 : 3;
        }
      }
      break;
    case AnimationMode::Tunnel:
      tunnelPhase = static_cast<uint8_t>((tunnelPhase + 2) % 66);
      break;
    case AnimationMode::Particles:
      ParticleSystem::update();
      break;
    case AnimationMode::GlitchText:
      glitchStrength = static_cast<uint8_t>(1 + ((now / 180UL) % 3UL));
      if ((now % 1900UL) < Config::FRAME_INTERVAL_MS) {
        glitchTextIndex = static_cast<uint8_t>(random(0, 5));
      }
      break;
    case AnimationMode::VisualPulse:
      visualPulsePhase += 7;
      break;
    case AnimationMode::Count:
      break;
  }
}

void render(AnimationMode mode, Adafruit_SSD1306& display) {
  switch (mode) {
    case AnimationMode::Equalizer:
      renderEqualizer(display, false);
      break;
    case AnimationMode::MirroredEqualizer:
      renderEqualizer(display, true);
      break;
    case AnimationMode::Pulse:
      renderPulse(display);
      break;
    case AnimationMode::Wave:
      renderWave(display);
      break;
    case AnimationMode::Starfield:
      renderStarfield(display);
      break;
    case AnimationMode::Tunnel:
      renderTunnel(display);
      break;
    case AnimationMode::Particles:
      ParticleSystem::render(display);
      break;
    case AnimationMode::GlitchText:
      renderGlitchText(display);
      break;
    case AnimationMode::VisualPulse:
      renderVisualPulse(display);
      break;
    case AnimationMode::Count:
      break;
  }
}

const __FlashStringHelper* modeName(AnimationMode mode) {
  switch (mode) {
    case AnimationMode::Equalizer:
      return F("Equalizer");
    case AnimationMode::MirroredEqualizer:
      return F("Mirrored equalizer");
    case AnimationMode::Pulse:
      return F("Pulse");
    case AnimationMode::Wave:
      return F("Wave");
    case AnimationMode::Starfield:
      return F("Starfield");
    case AnimationMode::Tunnel:
      return F("Tunnel");
    case AnimationMode::Particles:
      return F("Particles");
    case AnimationMode::GlitchText:
      return F("Glitch text");
    case AnimationMode::VisualPulse:
      return F("Visual pulse");
    case AnimationMode::Count:
      return F("None");
  }
  return F("Unknown");
}

}  // namespace Animations
