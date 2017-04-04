/**********************************************************************
  Sensor Datasheet:
    - EN: http://datasheet.sii-ic.com/en/temperature_sensor/S5851A_E.pdf
    - JP: http://datasheet.sii-ic.com/jp/temperature_sensor/S5851A_J.pdf
**********************************************************************/

#ifndef S5851A_H
#define S5851A_H

#include <stdbool.h>
#include <stdint.h>

class S5851A {
 private:
  int16_t _rawTemp;
  uint8_t _i2cAddress;

  bool generalCall(uint8_t value);
  bool write(uint8_t value);
  bool write(const uint8_t *values, size_t length);
  bool read(uint8_t *values, size_t length);
  bool readRegister(uint8_t address, uint8_t *value);
  bool readRegisters(uint8_t address, uint8_t *values, uint8_t length);
  bool writeRegister(uint8_t address, uint8_t value);

 public:
  S5851A(uint8_t i2cAddress);
  void begin();
  bool update();
  double getTempC();
  double getTempF();
  int16_t getRawTemp();
  bool resetByGeneralCall();
  bool reinstallAddressByGeneralCall();
  bool shutdown();
  bool startup();
};

#endif
