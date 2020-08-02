<h1 align="center">LedController</h1>

<h2 align="center">
<a href="https://github.com/noah1510/LedController/blob/master/License"><img src="https://img.shields.io/github/license/noah1510/LedController.svg" alt="GitHub license" /></a>
<img src="https://github.com/noah1510/LedController/workflows/pre-release/badge.svg" alt="pre-release" />
<a href="https://frontend.code-inspector.com/project/5318/dashboard"><img src="https://www.code-inspector.com/project/5318/score/svg" alt="code quality" /></a>
<a href="https://discord.gg/awjW3aG"><img src="https://discordapp.com/api/guilds/691968013234339881/widget.png" alt="Discord" /></a>
<a href="https://GitHub.com/noah1510/LedController/releases/"><img src="https://img.shields.io/github/v/release/noah1510/LedController.svg" alt="stable release" /></a>
</h2>

LedController is an [Arduino](http://arduino.cc) library for MAX7219 and MAX7221 Led display drivers.
The code should work with most boards and architectures but I only test it for the ESP32 Dev Module.
Feel free to create an issue if something is missing or not working correctly.

The CI automatically tests the following boards(PlatformIO boards names):

* esp32dev
* d1_mini
* uno
* ATmega1280
* leonardo

The following boards were able to compile on my machine once:

* nano_every

The following boards do **not** work at the moment:

* due
* genuino101

All help regarding compatibility with more boards is welcome.

## Important Notes

### Migration from 1.6.x to 1.7.x

There were a few more drastic changes in version 1.7.0 compared to 1.6.x, if you want use 1.7.x make sure your project is compatible with the latest version.
It is now possible to just create an empty LedController using `LedController()` and initilize it later using the `init` method.
View the documentation for more information.

There is no longer support for the use of std::array, so the functions that where able to return an ByteBlock, can no longer do that, so you have to pass a pointer to an ByteBlock as a secondary parameter.
Since there is no longer a limit on the amount of segments a controller can have, the size of most arrays is not known at compile time, and therefore all internal data has to be stored on the heap.
Please check the size of the arrays you pass as location to store the result in.
If it is now large enough, nothing will be displayed and fail silently.

## Documentation

Documentation for the library is on the [Github Project Pages](http://noah1510.github.io/LedController/)

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
