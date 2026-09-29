# Changelog

## [1.0.0] - 2026-09-29

First stable DIY release for the Arduino Nano ATmega328P and 128x64 SSD1306
I2C OLED reference assembly.

### Included

- Nine non-blocking procedural visual modes with safe transitions.
- 30 FPS timing with rollover-safe `millis()` scheduling.
- Configurable OLED address (`0x3C` or `0x3D`) and debug logging.
- I2C scanner and OLED primitive-test sketches.
- PlatformIO and Arduino IDE build instructions.
- Repeatable hardware qualification and 60-minute soak-test checklist.
- GitHub Actions builds for the firmware and both diagnostic sketches.

### Deferred

Audio reactivity, FFT, buttons, battery power, enclosure design, EEPROM
settings, and wireless features remain post-v1 roadmap items.
