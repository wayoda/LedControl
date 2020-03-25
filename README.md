# LedController [![GitHub license](https://img.shields.io/github/license/noah1510/LedController.svg)](https://github.com/noah1510/LedController/blob/master/License) ![pre-release](https://github.com/noah1510/LedController/workflows/pre-release/badge.svg)  [![code quality](https://www.code-inspector.com/project/5318/score/svg)](https://frontend.code-inspector.com/project/5318/dashboard) [![Discord](https://discordapp.com/api/guilds/691968013234339881/widget.png)](https://discord.gg/awjW3aG)

LedControl is an [Arduino](http://arduino.cc) library for MAX7219 and MAX7221 Led display drivers.
The code should work with most boards and architectures but I only test it for the ESP32 Dev Module.
Feel free to create an issue if something is missing or not working correctly.

## Documentation

Documentation for the library is on the [Github Project Pages](http://noah1510.github.io/LedController/)

## Download [![stable release](https://img.shields.io/github/v/release/noah1510/LedController.svg)](https://GitHub.com/noah1510/LedController/releases/)

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
