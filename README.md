# Pocket Rave

Pocket Rave is a four-wire Arduino OLED visualizer that automatically cycles
through procedural rave-inspired animations. The MVP targets an Arduino Uno or
Nano and a 0.96-inch 128x64 I2C SSD1306 display. It does not listen to music;
all motion is generated in code.

> **Photosensitivity warning:** this project contains moving, high-contrast
> monochrome patterns. Harsh full-screen strobing and display inversion are
> disabled by default. Stop using the device if the visuals cause discomfort.

## Features

- Non-blocking 30 FPS frame loop
- Non-blocking startup sequence with progress bar
- Random 8-12 second mode duration with no immediate repeats
- Equalizer, mirrored equalizer, pulse rings, dual wave, starfield, tunnel,
  particles, glitch text, and a gentle visual pulse
- Wipe transitions between modes
- Fixed-size arrays and no application-level dynamic allocation
- Compile-time configuration and optional once-per-second FPS logging
- Separate I2C scanner and OLED graphics test sketches

## Hardware

| Component | Quantity | Notes |
|---|---:|---|
| Arduino Uno or Nano (ATmega328P) | 1 | Nano suits the pocket form factor |
| 0.96-inch SSD1306 128x64 I2C OLED | 1 | Use the four-pin I2C version |
| Female-to-male jumper wires | 4 | GND, VCC, SDA, SCL |
| USB data cable | 1 | For programming and power |
| Breadboard | Optional | Useful during prototyping |

Confirm the OLED module's supported voltage before connecting VCC. Many
breakouts accept 3.3-5 V, but not all do.

The certified v1 reference is an ATmega328P Nano-compatible board and the
four-pin SSD1306 module described in [BOM.md](BOM.md). Uno compatibility is
retained as a documented alternative, but the qualification target is Nano.

## Wiring

Disconnect USB power before changing wires.

```text
OLED          ARDUINO UNO / NANO
GND   ------> GND
VCC   ------> 5V or 3.3V (check the module)
SDA   ------> A4
SCL   ------> A5
```

ESP32 boards often use GPIO 21 for SDA and GPIO 22 for SCL, but this firmware's
MVP is built and tested for Uno/Nano. See [WIRING.md](WIRING.md).

## Software setup

1. Install Arduino IDE 2.x.
2. In Library Manager, install **Adafruit SSD1306** and
   **Adafruit GFX Library**. Accept their dependencies.
3. Connect the board with a known data-capable USB cable.
4. Select **Arduino Uno** or **Arduino Nano** and its COM port.
5. For some clone Nanos, select **Processor > ATmega328P (Old Bootloader)** if
   uploads fail.
6. Upload Blink first to verify the board and USB connection.
7. Open `examples/I2CScanner/I2CScanner.ino`, upload it, and open Serial Monitor
   at 115200 baud. Most displays report `0x3C`; some report `0x3D`.
8. If necessary, change `Config::OLED_ADDRESS` in `Config.h`.
9. Optionally upload `examples/OLEDTest/OLEDTest.ino`.
10. Open `PocketRave.ino`, compile, and upload.

The project can also be built with PlatformIO:

```text
pio run -e nanoatmega328
pio run -d examples/I2CScanner
pio run -d examples/OLEDTest
```

The exact library versions used by the reproducible PlatformIO build are pinned
in `platformio.ini`. GitHub Actions rebuilds all three environments on every
push and pull request.

## Animation system

| Mode | Algorithm |
|---|---|
| Equalizer | Bars ease toward bounded random targets with slower peak decay |
| Mirrored equalizer | The same bar state is mirrored around the center line |
| Pulse | Offset concentric circles expand and wrap |
| Wave | Two integer triangle-wave traces move at different phase rates |
| Starfield | Fixed-point stars project outward as depth decreases |
| Tunnel | Nested rectangles expand around a slightly randomized vanishing point |
| Particles | Fixed-point particles burst from the center and respawn at bounds |
| Glitch text | Short text labels jitter with sparse bounded scan lines |
| Visual pulse | Nested rectangles breathe without full-screen flashing |

State update, framebuffer rendering, and OLED transfer are deliberately
separate:

1. `update()` advances values such as positions and phases.
2. `render()` draws the current state into the 1 KB RAM framebuffer.
3. `display()` transfers that framebuffer over I2C to the OLED.

## Configuration

Edit `Config.h` to change:

- `OLED_ADDRESS`
- `TARGET_FPS`
- minimum and maximum mode duration
- transition and startup timing
- debug output
- particle/star/bar counts
- safe visual-pulse and inversion policy

Increasing object counts consumes scarce ATmega328P SRAM. The display buffer
requires 1024 bytes out of 2048 bytes and is allocated at runtime by the
Adafruit library, so PlatformIO's static RAM summary does not include it. An
ESP32 is a better choice when adding audio buffers, FFT processing, many more
particles, Wi-Fi, or richer controls.

## Project structure

```text
PocketRave.ino             Setup, state, frame timing, FPS reporting
Config.h                   Hardware and behavior constants
DisplayManager.*           OLED initialization, startup, framebuffer transfer
AnimationManager.*         Mode and transition state machines
Animations.*               Animation state, update, and render functions
ParticleSystem.*           Fixed-size particle simulation
Utils.*                    Integer helpers and debug output
examples/I2CScanner/       Full address scanner sketch
examples/OLEDTest/         Static graphics verification sketch
```

## Testing

Run the scanner, OLED test, each animation, mode re-entry, and a continuous
60-minute soak test. The complete checklist is in [TESTING.md](TESTING.md).
Hardware runtime and thermal behavior cannot be proven without the physical
board, so record those results after flashing.

## Troubleshooting

If the OLED is blank, start with the scanner and check address, SDA/SCL order,
common ground, voltage, controller, and resolution. See
[TROUBLESHOOTING.md](TROUBLESHOOTING.md) for upload, flicker, pixel corruption,
and reset diagnostics.

## Known limitations

- Procedural graphics are not music-reactive.
- Uno/Nano SRAM limits visual complexity.
- I2C transfer time limits practical frame rate.
- There are no controls or persisted settings in Version 1.
- The default random seed provides variation, not secure randomness.

## Documentation

- [BOM.md](BOM.md) — reference parts, substitutions, and electrical constraints
- [PRD.md](PRD.md) — requirements, architecture, risks, and acceptance criteria
- [BUILD_GUIDE.md](BUILD_GUIDE.md) — staged build and upload procedure
- [WIRING.md](WIRING.md) — board wiring and electrical checks
- [TESTING.md](TESTING.md) — verification and soak-test checklist
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) — common failures and fixes
- [ROADMAP.md](ROADMAP.md) — controls, microphone, FFT, battery, and enclosure
- [RELEASE_CHECKLIST.md](RELEASE_CHECKLIST.md) — v1.0.0 sign-off and publication gates
- [CHANGELOG.md](CHANGELOG.md) — version history

## License and credits

The code is released under the MIT License; see [LICENSE](LICENSE). It uses the
Arduino framework and the Adafruit GFX and SSD1306 libraries. Project concept
and product direction were supplied by the Pocket Rave specification.
