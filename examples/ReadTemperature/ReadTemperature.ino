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
    - JP: http://akizukidenshi.com/download/ds/sii/S5851A_J.pdf
**********************************************************************/

#include <S5851A.h>
#include <Wire.h>

// Initialize sensor at I2C address 0x48
S5851A sensor(0x48);

void setup() {
  Serial.begin(9600);
  sensor.begin();

  delay(50);
}

void loop() {
  if (sensor.update()) {
    /* print format "RawSensorValue,Celsius,Fahrenheit" */

    // raw value by sensor
    // refer to "Table 10 Temperature Data" on datasheet.
    short rawTemp = sensor.getRawTemp();
    Serial.print(rawTemp);
    Serial.print(",");

    // degree Celsius calculated from raw value
    double tempC = sensor.getTempC();
    Serial.print(tempC);
    Serial.print(",");

    // degree Fahrenheit calculated from Celsius
    double tempF = sensor.getTempF();
    Serial.println(tempF);
  } else {
    Serial.println("update failed");
  }
  delay(1000);
}
