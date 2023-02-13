/******************************************************************
  @file       readTempAndPressure.ino
  @brief      Example sketch to read temperature & pressure
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

#include <ReefwingLPS22HB.h>

ReefwingLPS22HB LPS22HB;

void setup() {
    LPS22HB.begin();

    //  Start Serial and wait for connection
    Serial.begin(115200);
    while (!Serial);

    if (LPS22HB.connected()) {
        Serial.println("LPS22HB Pressure Sensor found.");
    }
    else {
        Serial.println("LPS22HB Pressure Sensor not found.");
        while (true) {  }   // loop forever
    }
}

void loop() {
    // Sensor Reading Loop
    Serial.print("Pressure: "); Serial.print(LPS22HB.readPressure()); Serial.print(" hPa, ");
    Serial.print("Temperature: "); Serial.print(LPS22HB.readTemperature()); Serial.print(" C, ");
    Serial.print("Altitude: "); Serial.println(" m");
    delay(1000);
}