#pragma once

#include <Arduino.h>

namespace Config {

constexpr int16_t SCREEN_WIDTH = 128;
constexpr int16_t SCREEN_HEIGHT = 64;
constexpr int8_t OLED_RESET_PIN = -1;
constexpr uint8_t OLED_ADDRESS = 0x3C;

constexpr uint32_t SERIAL_BAUD = 115200UL;
constexpr bool DEBUG_MODE = true;
constexpr uint8_t TARGET_FPS = 30;
constexpr uint32_t FRAME_INTERVAL_MS = 1000UL / TARGET_FPS;
constexpr uint16_t FPS_REPORT_INTERVAL_MS = 1000;

constexpr uint16_t STARTUP_DURATION_MS = 2600;
constexpr uint16_t MODE_DURATION_MIN_MS = 8000;
constexpr uint16_t MODE_DURATION_MAX_MS = 12000;
constexpr uint16_t TRANSITION_DURATION_MS = 500;

constexpr uint8_t EQUALIZER_BAR_COUNT = 12;
constexpr uint8_t STAR_COUNT = 14;
constexpr uint8_t PARTICLE_COUNT = 12;

// Keep full-screen flashing off for the safer default experience.
constexpr bool ENABLE_VISUAL_PULSE = true;
constexpr bool ALLOW_DISPLAY_INVERSION = false;

}  // namespace Config
