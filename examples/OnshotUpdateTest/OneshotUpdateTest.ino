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
}

void loop() {

  Serial.println("----- Check the time until oneshot update fished -----");
  if (sensor.requestOneshot()) {
    unsigned long startMillis = millis();
    int counter;
    for (counter = 0; counter < 500; counter++) {
      if (sensor.checkOneshotFinished()) {
        break;
      }
      delay(10);
    }
    if (counter < 500) {
      Serial.print("oneshot finished: ");
      Serial.print(millis() - startMillis);
      Serial.println(" ms");
      if (sensor.update()) {
        Serial.print("temperature: ");
        Serial.println(sensor.getTempC());
      } else {
        Serial.println("update() failed");
      }
    } else {
      Serial.println("Can't oneshot finished");
    }
  } else {
    Serial.println("requestOneshot() failed.");
  }

  Serial.println("");
  Serial.println("");

  delay(1000);

  Serial.println("----- Test waitForOneshotFinished() method -----");
  if (sensor.requestOneshot()) {
    if (sensor.waitForOneshotFinished(1000)) {
      if (sensor.update()) {
        Serial.print("temperature: ");
        Serial.println(sensor.getTempC());
      } else {
        Serial.println("update() failed");
      }
    } else {
      Serial.println("Timeout or can't read sensor register.");
    }
  } else {
    Serial.println("requestOneshot() failed.");
  }

  Serial.println("");
  Serial.println("");

  delay(1000);

  Serial.println("----- Check really shutdown mode -----");
  for (int i = 0; i < 10; i++) {
    if (sensor.update()) {
      Serial.print("temperature: ");
      Serial.println(sensor.getTempC());
    } else {
      Serial.println("update() failed");
    }
    delay(1000);
  }

  Serial.println("");
  Serial.println("");
  delay(1000);
}
