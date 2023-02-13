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
 * LPS22HB Implementation - 
 * 
 ******************************************************************/

ReefwingLPS22HB::ReefwingLPS22HB() : 
  _address(LPS22HB_ADDRESS),
  _rate((int)Rate::RATE_ONE_SHOT) { }

void ReefwingLPS22HB::begin() {
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

void ReefwingLPS22HB::setODR(Rate rate) {
  _rate = (int)rate;
  write(LPS22HB_CTRL_REG1, (_rate & 0x07) << 4);
}

void ReefwingLPS22HB::triggerOneShot() {
  write(LPS22HB_CTRL_REG2, 0x01);

  // wait for ONE_SHOT bit to clear
  while ((read(LPS22HB_CTRL_REG2) & 0x01) != 0) {
    yield();
  }
}

float ReefwingLPS22HB::readPressure(Units units) {
  if (_rate == RATE_ONE_SHOT) { triggerOneShot(); }
  
  uint8_t pressOutXL = read(LPS22HB_PRES_OUT_XL);
  uint8_t pressOutL = read(LPS22HB_PRES_OUT_L);
  uint8_t pressOutH = read(LPS22HB_PRES_OUT_H);
  
  long val = ( ((long)pressOutH << 16) | ((long)pressOutL << 8) | (long)pressOutXL ) / 4096.0f;
  
  switch (units) {
    case Units::HECTOPASCAL:
    case Units::MILLIBAR:
      //  Sensor returns value in hPa = mbar = 0.1 kPa = 0.0145 PSI
      break;
    case Units::KILOPASCAL:
      val = val * 0.1f;
      break;
    case Units::PSI:
      val = val * 0.0145037738;
      break;
  }

  return val;
}

uint32_t ReefwingLPS22HB::readPressureRAW() {
  if (_rate == RATE_ONE_SHOT) { triggerOneShot(); }
  
  uint8_t pressOutXL = read(LPS22HB_PRES_OUT_XL);
  uint8_t pressOutL = read(LPS22HB_PRES_OUT_L);
  uint8_t pressOutH = read(LPS22HB_PRES_OUT_H);

  int32_t val = ( (pressOutH << 16) | (pressOutL << 8) | pressOutXL );
  val = val + 0x400000;
  
  return (uint32_t)val;
}

float ReefwingLPS22HB::readTemperature() {
  if (_rate == RATE_ONE_SHOT) { triggerOneShot(); }

  uint8_t tempOutL = read(LPS22HB_TEMP_OUT_L);
  uint8_t tempOutH = read(LPS22HB_TEMP_OUT_H);
  int16_t val = (tempOutH << 8) | (tempOutL & 0xff);

  return ((float)val)/100.0f;
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


