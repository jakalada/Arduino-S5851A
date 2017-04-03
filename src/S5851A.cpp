/**********************************************************************
  Sensor Datasheet:
    - EN: http://datasheet.sii-ic.com/en/temperature_sensor/S5851A_E.pdf
    - JP: http://datasheet.sii-ic.com/jp/temperature_sensor/S5851A_J.pdf
**********************************************************************/

#include <Arduino.h>
#include <Wire.h>

#include "S5851A.h"

// Register Map
#define REG_TEMPERATURE 0x00
#define REG_CONFIGURATION 0x01

S5851A::S5851A(uint8_t i2cAddress) {
  _i2cAddress = i2cAddress;
  _rawTemp = 0;
}

void S5851A::begin() { Wire.begin(); }

bool S5851A::update() {
  uint8_t values[2];
  if (readRegisters(REG_TEMPERATURE, values, sizeof(values))) {
    int16_t rawValue = (int16_t)word(values[0], values[1]);
    rawValue >>= 4;
    _rawTemp = rawValue;
    return true;
  } else {
    return false;
  }
}

double S5851A::getTempC() { return (double)_rawTemp * 0.0625; }

double S5851A::getTempF() { return getTempC() * 9.0 / 5.0 + 32.0; }

int16_t S5851A::getRawTemp() { return _rawTemp; }

bool S5851A::write(uint8_t value) {
  uint8_t values[1] = {value};
  return write(values, 1);
}

bool S5851A::write(const uint8_t *values, size_t size) {
  Wire.beginTransmission(_i2cAddress);

  if (Wire.write(values, size) == size) {
    switch (Wire.endTransmission()) {
      case 0:  // success
        return true;

      case 1:  // data too long to fit in transmit buffer
        return false;

      case 2:  // received NACK on transmit of address
        return false;

      case 3:  // received NACK on transmit of data
        return false;

      case 4:  // other error
        return false;

      default:  // module unknown error
        return false;
    }
  } else {
    Wire.endTransmission();
    return false;
  }
}

bool S5851A::read(uint8_t *values, size_t size) {
  Wire.requestFrom(_i2cAddress, size);
  if (Wire.available() == size) {
    for (int8_t i = 0; i < size; i++) {
      values[i] = Wire.read();
    }
    return true;
  } else {
    return false;
  }
}

bool S5851A::readRegister(uint8_t address, uint8_t *value) {
  return readRegisters(address, value, 1);
}

bool S5851A::readRegisters(uint8_t address, uint8_t *values, uint8_t size) {
  if (!write(address)) {
    return false;
  }

  return read(values, size);
}

bool S5851A::writeRegister(uint8_t address, uint8_t value) {
  uint8_t values[2] = {address, value};
  return write(values, sizeof(values));
}
