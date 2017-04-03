/**********************************************************************
  Sensor I2C Address can be changed with an external jumper to:
    - 0x48: AD1 pin is VSS,   AD0 pin is VSS
    - 0x49: AD1 pin is VSS,   AD0 pin is Open
    - 0x4A: AD1 pin is VSS,   AD0 pin is VDD
    - 0x4C: AD1 pin is VDD,   AD0 pin is VSS
    - 0x4D: AD1 pin is VDD,   AD0 pin is Open
    - 0x4E: AD1 pin is VDD,   AD0 pin is VDD
    - 0x4B: AD1 pin is Open,  AD0 pin is VSS
    - 0x4F: AD1 pin is Open,  AD0 pin is VDD

  These value from "Table 11 Settings for Address Input Pin and Slave Address" on
  datasheet.

  Sensor Datasheet:
    - EN: http://datasheet.sii-ic.com/en/temperature_sensor/S5851A_E.pdf
    - JP: http://datasheet.sii-ic.com/jp/temperature_sensor/S5851A_J.pdf
**********************************************************************/

#include <S5851A.h>
#include <Wire.h>

// Initialize sensor at I2C address 0x48
S5851A sensor(0x48);

void setup() {
  Serial.begin(9600);
  sensor.begin();

  // Reset all connected S-5851A sensor by I2C General Call Address.
  // Refer to "8. General call" on datasheet.
  if (!sensor.resetByGeneralCall()) {
    Serial.println("failed: reset by general call");
    while(1) {};
  }
  delay(100);
}

void loop() {
  // Reinstall I2C address to all connected S-5851A sensor by I2C General Call Address.
  // Refer to "8. General call" on datasheet.
  if (sensor.reinstallAddressByGeneralCall()) {
    Serial.println("reinstall address.");
  } else {
    Serial.println("can't reinstall address.");
  }

  delay(50);

  if (sensor.update()) {
    Serial.println("update() success: I2C address is correct.");
  } else {
    Serial.println("update() failed: I2C address is incorrect. Probably AD0 and AD1 is wrong.");
  }
  Serial.println("");
  delay(1000);
}
