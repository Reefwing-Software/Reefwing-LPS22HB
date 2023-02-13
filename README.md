![version](https://img.shields.io/github/v/tag/Reefwing-Software/Reefwing-LPS22HB) ![license](https://img.shields.io/badge/license-MIT-green) ![release](https://img.shields.io/github/release-date/Reefwing-Software/Reefwing-LPS22HB?color="red") ![open source](https://badgen.net/badge/open/source/blue?icon=github)

# Reefwing LPS22HB
 
 Arduino Library for the LPS22HB Pressure Sensor, found in the Nano 33 BLE Sense Revisions 1 and 2. The LPS22HB is a compact piezoresistive absolute pressure sensor which functions as a digital barometer. The device comprises a sensing element and an interface which communicates using I2C or SPI. The Nano 33 BLE Sense is connected via I2C on Wire 1, and is factory calibrated.

 The LPS22HB has a 260 to 1260 hPa absolute pressure range, and can be sampled between 1 and 75 Hz (Output Data Rate = ODR = 1, 10, 25, 50 or 75). Within the range 800 - 1100 hPa, relative pressure accuracy is ±0.1 hPa.

 This library uses the barometer in one-shot mode, which is the default.

 ```
 Note that hectopascals (hPa) and millibars (mbar) are equivalent.

 1 hPa = 100 Pascals = 1 mbar

Following the adoption of the Pascal as the SI unit of pressure, 
meteorologists chose the hectopascal as the international unit for 
measuring atmospheric pressure. 

The millibar is still often used in weather reports and forecasts.
 ```

 The temperature sensor operates between -40 to 85°C and has an absolute accuracy of ±1.5°C. ODR for the temperature sensor is the same as for the pressure sensor.

 This library was originally contained within the Reefwing AHRS library but was broken out due to the release of Rev. 2 of the Nano 33 BLE Sense. This new version of the Sense board had updated sensors which were different to Rev. 1.

## Control Register 2 (0x11)

The 8-bit control register 2 (`CTRL_REG2`), is used to set the data acquisition mode of the barometer. To request a new dataset, all bits are set to 0 apart from bit 0 (`ONE_SHOT`), which is set to 1.

```c++
    write(LPS22HB_CTRL_REG2, 0x01);
```

If the `ONE_SHOT` bit in `CTRL_REG2` (0x11) is set to '1', one-shot mode is triggered and a
new acquisition starts. Enabling this mode is possible only if the device
was previously in power-down mode (ODR bits set to '000'). Once the acquisition is
completed and the output registers updated, the device automatically enters power-down
mode and the `ONE_SHOT` bit clears itself. 

## Interpreting Pressure Readings

The pressure data is stored in 3 registers: `PRESS_OUT_H` (2Ah), `PRESS_OUT_L` (29h),
and `PRESS_OUT_XL` (28h). The value is expressed as a 2’s complement. To obtain the pressure in hPa, take the two’s complement of the complete word and then divide by 4096 LSB/hPa.

```c++
    uint8_t pressOutH = read(LPS22HB_PRES_OUT_H);
    uint8_t pressOutL = read(LPS22HB_PRES_OUT_L);
    uint8_t pressOutXL = read(LPS22HB_PRES_OUT_XL);

    long val = ( ((long)pressOutH << 16) | ((long)pressOutL << 8) | (long)pressOutXL );
  
    return val/4096.0f;
```

## The STATUS Register (0x27)
The LPS22HB 8-bit `STATUS` Register, located at address 0x27 contains two bits that we are interested in, `P_DA` (BIT 0) and `T_DA` (BIT 1). This register is updated every ODR cycle.

- `P_DA`: Is the Pressure Data Available bit, it is 1 when new pressure data is available and 0 otherwise.
- `T_DA`: Is the Temperature Data Available bit, it is 1 when new temperature data is available and 0 otherwise.

The status()

## Determing Altitude from Air Pressure



## Using the Library


