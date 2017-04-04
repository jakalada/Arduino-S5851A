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
  // Shutdown
  // Refer to "4.1 Shutdown mode (SD)" on datasheet.
  if (sensor.shutdown()) {
    Serial.println("Shutdown. Check that temperature value is not updated even if the surrounding environment changes.");
  } else {
    Serial.println("can't reinstall address.");
  }

  // Shutdown mode is valid after the current conversion.
  // So delay for conversio.
  // Refer to "Characteristics (Typical Data)" on datasheet.
  delay(400);

  for (int i = 0; i < 10; i++) {
    if (sensor.update()) {
      Serial.println(sensor.getTempC());
    } else {
      Serial.println("update() failed");
    }
    delay(1000);
  }

  // Startup
  // Refer to "4.1 Shutdown mode (SD)" on datasheet.
  if (sensor.startup()) {
    Serial.println("Startup. Check that temperature value is updated if the surrounding environment changes.");
  } else {
    Serial.println("can't reinstall address.");
  }

  for (int i = 0; i < 10; i++) {
    if (sensor.update()) {
      Serial.println(sensor.getTempC());
    } else {
      Serial.println("update() failed");
    }
    delay(1000);
  }

  delay(1000);
}
