# How to use this library

**This usage introduction is still work in progress. For more details about the LedController and its functions click [here](d9/def/class_led_controller.html) or the [examples](https://github.com/noah1510/LedController/tree/master/examples).**

This page shows you the basic usage of the LedController and what you can do with it.

## Initilizing the Library

You can initilize the library in two ways: with hardware SPI or with software SPI.

1. Hardware SPI is at least 10 times faste but you can only use specific pins for some signals (look [here](index.html) for which pins).
2. Software SPI is slower (which only mattesrs if you need really tight timings) but you can freely choose which (free) pins you want to use.

### Small introduction to SPI

The Serial Peripheral Interface (SPI) is a synchronous serial communication interface specification used for short-distance communication, primarily in embedded systems.
In this case your microcontroller is the master which controls the slave (at least one 7-Segment display or led matrix).

The master has three significant pins: SCLK(Serial Clock), MOSI(Master Out Slave In), SS(Slave Select).
The slave also has tree needed pins (and two for power(GND and VCC)): CLK(Clock), DIN(Data In), CS(Ship Select).
SCLK has to be connected to CLK and MOSI to DIN.
These signals can be shared by multiple SPI slaves even if they have nothing to do with each other.
SS hat to be connected to CS and each Slave has its own SS pin(which can be any free pin).

### Software SPI

The constructor for the LedController needs at least 3 arguments but accepts five.

1. unsigned int dataPin: The Pin which should be used for the MOSI signal.
2. unsigned int clkPin: The pin which should be used for the SCLK signal.
3. unsigned int csPin: The pin which should be used for the SS signal.
4. (optional default 4) unsigned int numSegments: if your amount of segments differs from 4, specify the number.
5. (optional default false) bool useHardwareSpi: set to true if you want to use hardware SPI with the specified ports.

Simply declare a global object with these parameters and you are ready to go.

View [here](d9/def/class_led_controller.html#aaf70bc009abce437e5d07e23ba23535f) for more information.

### Hardware SPI

The constructor for the default SPI ports (SPI interface 1) are used by this constructor.
If you want to use different pins use the constructor for Software SPI with useHardwareSpi set to true(***please note that the number of segments has to be manually specified in this case***).

This constructor only needs one argument but accepts two.

1. unsigned int csPin: The pin which should be used for the SS signal.
2. (optional default 4) unsigned int numSegments: if your amount of segments differs from 4, specify the number.

Simply declare a global object with these parameters and you are ready to go.

View [here](d9/def/class_led_controller.html#a48414cff463afcec44e3056f9a84fac4) for more information.
