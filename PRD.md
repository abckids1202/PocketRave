# Pocket Rave product requirements

## Product summary

Pocket Rave is a small, USB-powered embedded visualizer that generates smooth
rave-inspired monochrome graphics on a 128x64 I2C OLED using an Arduino Uno or
Nano. Version 1 teaches I2C, framebuffer graphics, procedural animation,
non-blocking timing, fixed-size state, and finite-state-machine design.

The MVP is procedural and is **not** music-reactive. Real audio response requires
at least a microphone/audio input and is deferred.

## Users and goals

The primary user is a beginner-to-intermediate Arduino developer. Secondary
users include students, wearable/desk-toy makers, and developers learning
embedded graphics.

Goals:

- Run automatically after power-on with four OLED wires.
- Sustain about 30 FPS without obvious freezing or flicker.
- Provide at least six polished modes; eight or more are preferred.
- Remain maintainable, bounds-conscious, and feasible on ATmega328P SRAM.
- Be easy to rebuild, test, and extend.

Non-goals for Version 1 include Wi-Fi, Bluetooth, phone control, audio analysis,
buttons, battery charging, file storage, full color, and accurate 3D rendering.

## Functional requirements

| ID | Requirement |
|---|---|
| FR-01 | Initialize serial, I2C, and OLED; fail safely; show a non-blocking startup |
| FR-02 | Render near 30 FPS using `millis()` timing |
| FR-03 | Randomly cycle modes every 8-12 seconds without immediate repeats |
| FR-04 | Render smooth procedural equalizer bars with peak decay |
| FR-05 | Render expanding layered pulse circles |
| FR-06 | Render moving multi-wave graphics |
| FR-07 | Render a depth-based starfield with bounded respawn |
| FR-08 | Render a looping geometric tunnel |
| FR-09 | Update/render a fixed-size particle array without heap allocation |
| FR-10 | Render animated glitch text with bounded random variation |
| FR-11 | Provide a safe visual pulse; harsh strobe remains disabled |
| FR-12 | Transition between modes without blocking |
| FR-13 | Compile-time debug mode reports modes and FPS at a limited rate |

## Non-functional requirements

- Fixed arrays; no application `new`, `delete`, or Arduino `String`.
- Rollover-safe unsigned timing and no normal-operation blocking delays.
- Separate update, render, and framebuffer-transfer responsibilities.
- Handle OLED initialization failure.
- Keep all indices bounded and drawing coordinates clipped or safely handled by
  Adafruit GFX.
- Store fixed debug strings with `F()` where practical.
- Document electrical uncertainty rather than assuming every OLED is 5 V safe.

## Architecture

```text
PocketRave.ino
  |-- frame/startup state
  |-- AnimationManager (mode + transition FSM)
  |     |-- Animations (mode state/update/render)
  |     `-- ParticleSystem
  `-- DisplayManager (SSD1306 + framebuffer transfer)
         `-- Adafruit GFX / SSD1306 / Wire
```

The SSD1306 framebuffer consumes 1024 bytes, half of an ATmega328P's 2 KB SRAM,
so small fixed-point structures replace large floating-point/local buffers.

## Milestones

1. Verify board with Blink.
2. Detect OLED using the scanner.
3. Verify graphics primitives using OLEDTest.
4. Verify startup and stable frame loop.
5. Verify every animation and repeat entry.
6. Verify randomized mode selection and transitions.
7. Review compiler flash/SRAM report and warnings.
8. Complete the 60-minute physical soak test.

## Risks and mitigations

| Risk | Impact | Mitigation |
|---|---|---|
| Wrong OLED address/controller | Blank display | Scanner, configurable address, controller checklist |
| 3.3/5 V mismatch | Hardware damage | Require module-voltage verification |
| ATmega328P SRAM exhaustion | Resets/corruption | Fixed small arrays, no `String`, compiler memory review |
| I2C transfer too slow | Low FPS | One transfer/frame; configurable FPS |
| Bad random coordinates | Corruption | Fixed-width state, bounded loops, explicit boundary reset |
| Photosensitive response | User discomfort | Warning; no rapid inversion/full-screen strobe by default |
| Clone-board upload trouble | Cannot flash | Blink gate, cable/driver/old-bootloader guidance |

## Acceptance criteria

- Four-wire SSD1306 OLED setup is documented and detected.
- Startup and at least eight animation variants run automatically.
- Mode timing and transitions are non-blocking.
- Measured frame rate is approximately 30 FPS on selected hardware.
- There is no major flicker, array overflow, or dynamic application allocation.
- Debug output and OLED address are configurable.
- Strong strobe/inversion remains disabled by default.
- The documented build compiles for the selected Uno/Nano toolchain.
- A physical unit passes the checklist and a 60-minute soak test.

The final two criteria that depend on actual hardware must be signed off after
upload; source inspection alone cannot establish thermal, wiring, or endurance
behavior.
