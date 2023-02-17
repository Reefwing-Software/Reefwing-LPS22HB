/******************************************************************
  @file       readAltitude.ino
  @brief      Example sketch to read altitude
  @author     David Such
  @copyright  Please see the accompanying LICENSE file.

  Code:        David Such
  Version:     1.0.0
  Date:        11/02/23

  1.0.0 Original Release.                         11/02/23

  Acquires temperature and pressure readings from the LPS22HB
  Pressure Sensor mounted on the Arduino Nano 33 BLE Sense
  boards Rev. 1 and 2. Altitude is calculated using the barometric
  equation and is relative to the reference pressure passed into
  the method.

  Credit - LPS22HB Absolute Digital Barometer class 
           based on work by Adrien Chapelet for IoThings.
           ref: https://github.com/adrien3d/IO_LPS22HB

******************************************************************/


