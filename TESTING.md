# Testing checklist

Record board model, OLED model/address, supply voltage, IDE/core versions,
library versions, test date, and tester before running this checklist.

## Hardware

- [ ] Board powers on without excessive heat
- [ ] Blink compiles and uploads
- [ ] USB cable carries data
- [ ] Correct board, processor, and COM port are selected
- [ ] Scanner detects the OLED at `0x3C` or `0x3D`
- [ ] GND, VCC, SDA, and SCL remain mechanically stable
- [ ] Display orientation is correct
- [ ] No random resets occur

## Display primitives

- [ ] Pixel
- [ ] Horizontal line
- [ ] Vertical line
- [ ] Rectangle
- [ ] Filled rectangle/full-screen fill
- [ ] Circle
- [ ] Text
- [ ] Clear framebuffer
- [ ] Inversion tested briefly only if safe for the tester

## Firmware and animation

- [ ] Startup stages advance without blocking
- [ ] Equalizer rises, decays, and holds peaks
- [ ] Mirrored equalizer remains within both halves
- [ ] Pulse circles reset cleanly
- [ ] Waves remain within the screen
- [ ] Starfield respawns without stuck or wrapped coordinates
- [ ] Tunnel layers loop and clip cleanly
- [ ] Particles respawn at lifetime or screen boundary
- [ ] Glitch text stays bounded and legible
- [ ] Visual pulse does not produce harsh full-screen flashes
- [ ] Every mode can be entered, exited, and entered again
- [ ] Immediate mode repeats do not occur
- [ ] Transitions complete and do not corrupt mode state

## Timing and memory

- [ ] Average FPS remains close to the configured target
- [ ] Mode duration remains between 8 and 12 seconds, excluding transitions
- [ ] Serial logging occurs about once per second, not once per frame
- [ ] No long blocking calls exist in normal operation
- [ ] `millis()` comparisons use unsigned subtraction
- [ ] Build output fits flash and SRAM, accounting separately for the Adafruit
      library's runtime-allocated 1024-byte framebuffer
- [ ] Compiler warnings are reviewed

## 60-minute soak test

Run continuously on the final wiring for at least 60 minutes.

- [ ] No freeze
- [ ] No spontaneous reset
- [ ] No slowdown
- [ ] No broken mode switch
- [ ] No persistent OLED artifact
- [ ] No loose-wire interruption
- [ ] No excessive heat or odor

Hardware tests require the physical device and are intentionally not claimed by
the source-only build. If any soak item fails, record the time, visible mode,
serial output, power source, and component temperature before resetting.
