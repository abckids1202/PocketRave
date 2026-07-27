# Build guide

## Phase 1: verify the board

Install Arduino IDE 2.x, connect the board with a USB data cable, select the
correct board and COM port, and upload the built-in Blink example. If a Nano
clone fails, try the old bootloader processor option and install the correct
USB-serial driver (often CH340).

Expected result: Blink uploads and the onboard LED changes state. Do not debug
the OLED until this passes.

## Phase 2: verify the OLED

Disconnect power and wire the four pins as described in `WIRING.md`. Upload
`examples/I2CScanner/I2CScanner.ino`, open Serial Monitor at 115200 baud, and
record the address. Set that address in both the OLED test and `Config.h`.

Upload `examples/OLEDTest/OLEDTest.ino`. It draws text, a pixel, horizontal and
vertical lines, a rectangle, a circle, and a filled rectangle.

Expected result: stable, correctly oriented graphics without random pixels,
reset loops, heat, or flicker.

## Phase 3: upload Pocket Rave

Install the Adafruit SSD1306 and Adafruit GFX libraries in Library Manager. Open
`PocketRave.ino`, select Uno or Nano, click Verify, then Upload.

Expected sequence:

1. WAKE scan line
2. POCKET RAVE title
3. SYSTEM READY progress bar
4. Equalizer mode
5. Random non-repeating modes every 8-12 seconds

Serial Monitor at 115200 baud reports initialization, mode name, duration, and
measured FPS once per second.

## Frame timing

The main loop uses rollover-safe unsigned subtraction and only renders when a
frame interval is due. State update, framebuffer drawing, and OLED transfer
remain separate. There is no mode or startup `delay()`. The fixed interval is
advanced rather than replaced with the current time to reduce drift; a
three-frame guard recovers if the board falls far behind.

## Validation by stage

For each mode, confirm first entry, at least one complete cycle, exit, and
re-entry. Check Serial Monitor for approximately 30 FPS. If the display cannot
keep up, lower `TARGET_FPS` to 25 rather than allowing a busy unstable loop.

Complete the 60-minute physical soak test in `TESTING.md` before declaring a
specific hardware assembly production-ready.
