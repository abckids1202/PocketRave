# PocketRave v1.0.0 bill of materials

This is the reference bill of materials for the qualified DIY assembly.

| Item | Qty | Required specification | Acceptable substitution |
|---|---:|---|---|
| Arduino Nano-compatible board | 1 | ATmega328P, 5 V, 16 MHz, USB data connection | Official Nano or Uno R3; Uno is documented but not the reference target |
| OLED module | 1 | SSD1306, 128x64, four-pin I2C, monochrome | Equivalent SSD1306 I2C breakout with verified controller and address |
| Jumper wires | 4 | Female-to-male, secure contacts | Short soldered wires in an enclosure |
| USB cable | 1 | Data-capable cable matching the board connector | A known-good replacement cable |
| Breadboard | 1 optional | Prototyping layout | Point-to-point wiring with strain relief |

## Electrical constraints

- Verify the OLED breakout's VCC rating before applying power. Use 3.3 V when
  the module is not explicitly rated for 5 V.
- Connect grounds together before connecting VCC.
- Disconnect USB power before changing wiring.
- Do not connect a bare LiPo cell to this v1 assembly.

## Reference wiring

| OLED | Nano | Purpose |
|---|---|---|
| GND | GND | Common ground |
| VCC | 5V or 3.3V per module rating | Supply |
| SDA | A4 | I2C data |
| SCL | A5 | I2C clock |
