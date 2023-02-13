/******************************************************************
  @file       ReefwingLPS22HB.h
  @brief      Arduino Library for the LPS22HB Pressure Sensor
  @author     David Such
  @copyright  Please see the accompanying LICENSE file.

  Code:        David Such
  Version:     1.0.0
  Date:        11/02/23

  1.0.0 Original Release.                         11/02/23

  Credit - LPS22HB Absolute Digital Barometer class 
           based on work by Adrien Chapelet for IoThings.
           ref: https://github.com/adrien3d/IO_LPS22HB

******************************************************************/

#ifndef ReefwingLPS22HB_h
#define ReefwingLPS22HB_h

#include <Arduino.h>

/******************************************************************
 *
 * LPS22HB Class Definition - 
 * 
 ******************************************************************/

class ReefwingLPS22HB {
public:
  ReefwingLPS22HB();

  void begin();
  void reset();
  bool connected();

  uint8_t whoAmI();
  float readTemperature();

  float readPressure();
  uint32_t readPressureUI();
  uint32_t readPressureRAW();

private:
  uint8_t _address;
  uint8_t read(uint8_t reg);
  void write(uint8_t reg, uint8_t data);
  uint8_t status(uint8_t status_bit);
};

#endif