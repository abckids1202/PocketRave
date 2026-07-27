# Troubleshooting

## OLED remains blank

1. Run the I2C scanner.
2. If nothing is found, disconnect power and check common ground, VCC, pin
   order, and swapped SDA/SCL.
3. If `0x3D` is found, update `Config::OLED_ADDRESS`.
4. Confirm the module is SSD1306, 128x64, I2C—not SH1106, 128x32, or SPI.
5. Confirm the voltage permitted by the module.
6. Upload the standalone OLED test.

## Upload fails

- Select the correct board and COM port.
- Close other programs holding the serial port.
- Confirm the cable supports data.
- For Nano clones, try ATmega328P (Old Bootloader).
- Install the board's USB-serial driver, often CH340.
- Retry Blink to separate upload trouble from project trouble.

## Random pixels or corrupted graphics

Check loose wiring and unstable power first. Then confirm the correct controller
and resolution. If corruption appears only after one mode, check array bounds,
signed coordinate arithmetic, and SRAM usage in the compiler report.

## Animation flickers or runs slowly

Do not call `display.display()` more than once per frame. Keep debug output to
the existing once-per-second report. Lower `TARGET_FPS` to 25 if a slow I2C
module cannot sustain 30 FPS. Avoid reinitializing the display inside `loop()`.

## Device resets

Disconnect immediately if wiring or components become hot. Check for shorts and
power instability. Review SRAM use, large local arrays, out-of-bounds writes,
and stack growth. Restore the default object counts before adding features.

## Nothing changes after the startup screen

Open Serial Monitor at 115200 baud. A display initialization failure stops
safely and prints a diagnostic. If mode messages continue but graphics do not,
upload the OLED test and inspect the display transfer/wiring.
