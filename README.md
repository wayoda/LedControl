# LedController

[![GitHub license](https://img.shields.io/github/license/noah1510/LedController.svg)](https://github.com/noah1510/LedController/blob/master/License)
![pre-release](https://github.com/noah1510/LedController/workflows/pre-release/badge.svg)
[![code quality](https://api.codiga.io/project/8566/score/svg)](https://app.codiga.io/hub/project/8566/LedController)
[![stable release](https://img.shields.io/github/v/release/noah1510/LedController.svg)](https://GitHub.com/noah1510/LedController/releases/)
[![arduino-library-badge](https://www.ardu-badge.com/badge/LedController.svg?)](https://www.ardu-badge.com/LedController)

LedController is an [Arduino](http://arduino.cc) library for MAX7219 and MAX7221 Led display drivers.
The code should work with most boards and architectures but I only test it for the ESP32 Dev Module.
Feel free to create an issue if something is missing or not working correctly.

Please note that while 7-Segment Displays are supported, most functions are intended for use with LED-Matrices and are not tested with 7-Segment Displays.
Testing and reporting of problems or even fixes are welcome.

## Issues, help and contributer information

Whatever questions or suggestions you have, please provide detailed information about what you want, what went wrong and what your environment is.

If you have a feature request or find issues, feel free to open an issue on github.

For questions that are not bugs or feature requests please start a new [discussion](https://github.com/noah1510/LedController/discussions).

Also help with more translations is welcome.
At the moment the documentation is in German and English.

## Supported boards

The CI automatically tests the following boards(PlatformIO boards names):

- esp32dev
- d1_mini
- uno
- ATmega1280
- leonardo
- due (since 2.0.0)
- teensy41 (since 2.0.0)
- bluefruitmicro (since 2.0.0)

All help regarding compatibility with more boards is welcome.

## Important Notes

You might need changes when migrating with the following transitions:

* 1.6.x to 1.7.x
* 1.x.y to 2.0.x

Since 2.0.0 is a huge release you will definatly need changes to your code to get it working with that version.
For information on what needs to be done to transition from one version to another check out the [migration notes](https://noah1510.github.io/LedController/english/d9/dbb/migration_notes.html)

## Documentation

Documentation for the library is on the [Github Project Pages](http://noah1510.github.io/LedController/index.html).
It is available in [German](http://noah1510.github.io/LedController/german/index.html) and [English](http://noah1510.github.io/LedController/english/index.html).

## Download

The lastest version of the Library is always available from the [LedController Release Page](https://github.com/noah1510/LedController/releases)

The library can be also installed using the [standard Arduino library install procedure](http://arduino.cc/en/Guide/Libraries).

## Supported Editors

The library includes a keywords.txt for the [Arduino ide](https://www.arduino.cc/) and a configuration for visual studio code (with the [platform.io extention](https://platformio.org/platformio-ide)).

## Hardware SPI Pin Assignments

If you want to use hardware SPI you have to use the following port.
Please keep in mind that the MOSI(Master out Slave in) and the SCK(Serial Clock) signal can be shared by multiple SPI devices, while the SS(Slave Select) signal must be unique to every device(You can use every free pin for this).

| Arduino Board | MOSI | MISO | SCK | SS (Slave) | SS (Master) |
|---------------|------|------|-----|------------|-------------|
| Uno or Duemilanove | 11 or ICSP-4 | 12 or ICSP-1 | 13 or ICSP-3 | 10 | - |
| Mega1280 or Mega2560 | 51 or ICSP-4 | 50 or ICSP-1 | 52 or ICSP-3 | 53 | - |
| Leonardo | ICSP-4 | ICSP-1 | ICSP-3 | - | - |
| Due | ICSP-4 | ICSP-1 | ICSP-3 | - | 4, 10, 52 |
| Deek-Robot Arduino | 11 | 12 | 13 | - | 10 |
| ESP32-WROOM | 23 | 19 | 18 | - | 5 |
