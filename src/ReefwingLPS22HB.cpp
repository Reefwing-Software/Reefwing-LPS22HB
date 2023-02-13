/******************************************************************
  @file       ReefwingLPS22HB.cpp
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

#include <Arduino.h>
#include <Wire.h>

#include "ReefwingLPS22HB.h"

/******************************************************************
 *
 * Barometer Registers -
 * 
 ******************************************************************/

#define LPS22HB_WHO_AM_I        0X0F // Who am I
#define LPS22HB_RES_CONF        0X1A // Normal (0) or Low current mode (1)
#define LPS22HB_CTRL_REG1       0X10 // Output rate and filter settings
#define LPS22HB_CTRL_REG2       0X11 // BOOT FIFO_EN STOP_ON_FTH IF_ADD_INC I2C_DIS SWRESET One_Shot
#define LPS22HB_STATUS_REG      0X27 // Temp or Press data available bits
#define LPS22HB_PRES_OUT_XL     0X28 // XLSB
#define LPS22HB_PRES_OUT_L      0X29 // LSB
#define LPS22HB_PRES_OUT_H      0X2A // MSB
#define LPS22HB_TEMP_OUT_L      0X2B // LSB
#define LPS22HB_TEMP_OUT_H      0X2C // MSB
#define LPS22HB_WHO_AM_I_VALUE  0xB1 // Expected return value of WHO_AM_I register

/******************************************************************
 *
 * I2C Device Addresses - 
 * 
 ******************************************************************/

#define LPS22HB_ADDRESS   0x5C  //  Address of barometer (Nano 33 BLE SENSE) 
#define DEFAULT_ADDRESS   0x5D  //  Sensor I2C Default Address

/******************************************************************
 *
 * Helper Defines - 
 * 
 ******************************************************************/

#define NO_DATA_F     1.23
#define NO_DATA_UI    123
#define NO_DATA_T     456.0

/******************************************************************
 *
 * LPS22HB Implementation - 
 * 
 ******************************************************************/

ReefwingLPS22HB::ReefwingLPS22HB() { }

void ReefwingLPS22HB::begin() {
  _address = LPS22HB_ADDRESS;
  Wire1.begin();
  write(LPS22HB_RES_CONF, 0x00);    // resolution: temp=32, pressure=128
  write(LPS22HB_CTRL_REG1, 0x00);   // one-shot mode
}

void ReefwingLPS22HB::reset() {
  write(LPS22HB_CTRL_REG2, 0x04);   // software reset. Self clears when reset completes
}

byte ReefwingLPS22HB::whoAmI() {
  Wire1.beginTransmission(_address);
  Wire1.write(LPS22HB_WHO_AM_I);
  Wire1.endTransmission();
  Wire1.requestFrom(_address, 1);
  return Wire1.read();
}

bool ReefwingLPS22HB::connected() {
  return (whoAmI() == LPS22HB_WHO_AM_I_VALUE);
}

float ReefwingLPS22HB::readPressure() {
  write(LPS22HB_CTRL_REG2, 0x01);

  if (status(0x01) < 0)
    return NO_DATA_F;
  
  uint8_t pressOutH = read(LPS22HB_PRES_OUT_H);
  uint8_t pressOutL = read(LPS22HB_PRES_OUT_L);
  uint8_t pressOutXL = read(LPS22HB_PRES_OUT_XL);

  long val = ( ((long)pressOutH << 16) | ((long)pressOutL << 8) | (long)pressOutXL );
  
  return val/4096.0f;
}

uint32_t ReefwingLPS22HB::readPressureRAW() {
  write(LPS22HB_CTRL_REG2, 0x01);

  if (status(0x01) < 0)
    return NO_DATA_UI;
  
  uint8_t pressOutH = read(LPS22HB_PRES_OUT_H);
  uint8_t pressOutL = read(LPS22HB_PRES_OUT_L);
  uint8_t pressOutXL = read(LPS22HB_PRES_OUT_XL);

  int32_t val = ( (pressOutH << 16) | (pressOutL << 8) | pressOutXL );
  val=val+0x400000;
  
  return (uint32_t)val;
}

uint32_t ReefwingLPS22HB::readPressureUI() {
  write(LPS22HB_CTRL_REG2, 0x01);

  if (status(0x1) < 0)
    return NO_DATA_UI;
  
  uint8_t pressOutH = read(LPS22HB_PRES_OUT_H);
  uint8_t pressOutL = read(LPS22HB_PRES_OUT_L);
  uint8_t pressOutXL = read(LPS22HB_PRES_OUT_XL);
  uint32_t val = ((pressOutH << 16) | (pressOutL << 8) | pressOutXL );
  
  return val/4096;
}

float ReefwingLPS22HB::readTemperature() {
  write(LPS22HB_CTRL_REG2, 0x01);
  if (status(0x02) < 0)
    return NO_DATA_T;

  uint8_t tempOutH = read(LPS22HB_TEMP_OUT_H);
  uint8_t tempOutL = read(LPS22HB_TEMP_OUT_L);
  int16_t val = (tempOutH << 8) | (tempOutL & 0xff);

  return ((float)val)/100.0f;
}


uint8_t ReefwingLPS22HB::status(uint8_t status_bit) {
  //  Check for available data at status_bit count times
  int count = 1000;
  uint8_t data = 0x00;

  do {
    data = read(LPS22HB_STATUS_REG);
    --count;
    if (count < 0)
      break;
  } while ((data & status_bit) == 0);

  if (count < 0)
    return -1;
  else
    return 0;
}

uint8_t ReefwingLPS22HB::read(uint8_t reg) {
  Wire1.beginTransmission(_address);
  Wire1.write(reg);
  Wire1.endTransmission();
  Wire1.requestFrom(_address, 1);
  return Wire1.read();
}

void ReefwingLPS22HB::write(uint8_t reg, uint8_t data) {
  Wire1.beginTransmission(_address);
  Wire1.write(reg);
  Wire1.write(data);
  Wire1.endTransmission();
}


