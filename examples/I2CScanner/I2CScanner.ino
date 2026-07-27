#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (!Serial) {
    // Needed on native-USB boards; Uno/Nano continue immediately.
  }
  Serial.println(F("Pocket Rave I2C scanner"));
}

void loop() {
  uint8_t devicesFound = 0;

  for (uint8_t address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    const uint8_t result = Wire.endTransmission();

    if (result == 0) {
      Serial.print(F("I2C device found at 0x"));
      if (address < 16) {
        Serial.print('0');
      }
      Serial.println(address, HEX);
      ++devicesFound;
    } else if (result == 4) {
      Serial.print(F("Unknown I2C error at 0x"));
      Serial.println(address, HEX);
    }
  }

  if (devicesFound == 0) {
    Serial.println(F("No I2C devices found"));
  }
  Serial.println(F("Scanning again in 3 seconds"));
  delay(3000);
}
