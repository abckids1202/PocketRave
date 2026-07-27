# Wiring and electrical setup

## Default Uno/Nano connection

```text
SSD1306 OLED      Arduino Uno/Nano
-----------       ----------------
GND          ---> GND
VCC          ---> 5V or 3.3V
SDA          ---> A4
SCL          ---> A5
```

Use a four-pin **I2C** SSD1306 module. An SPI module has extra pins and is not a
drop-in replacement. Read the module label or seller specification before
choosing VCC; a 3.3 V-only display can be damaged by 5 V.

## Safe wiring procedure

1. Unplug USB and any battery.
2. Verify the OLED pin order printed on the module. Pin order varies.
3. Connect GND first, then VCC, SDA, and SCL.
4. Check for adjacent loose strands or reversed VCC/GND.
5. Reconnect USB.
6. Upload the scanner sketch and check Serial Monitor at 115200 baud.

The expected address is usually `0x3C`, occasionally `0x3D`. Update
`Config::OLED_ADDRESS` if the scanner reports the latter.

## Other boards

| Board | SDA | SCL | Logic level |
|---|---|---|---|
| Uno/Nano | A4 | A5 | 5 V |
| ESP32 (common default) | GPIO 21 | GPIO 22 | 3.3 V |
| Raspberry Pi Pico Arduino core | Board/core dependent | Board/core dependent | 3.3 V |

Always confirm the exact board variant's pinout. Do not assume a 5 V OLED supply
also means its I2C logic is safe for every 3.3 V microcontroller.

## Power notes

The USB-powered MVP needs no battery circuit. Do not connect a bare LiPo
directly without verifying voltage regulation, charging, and protection. A
portable version should use a protected cell, suitable charger, regulator when
required, insulated contacts, and a physical switch.
