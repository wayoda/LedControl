# How to use this library

**For more details about the LedController and its functions click [here](d9/def/class_led_controller.html) or the [examples](https://github.com/noah1510/LedController/tree/master/examples).**

This page shows you the basic usage of the LedController and what you can do with it.

## Initilizing the Library

You can initilize the library in two ways: with hardware SPI or with software SPI.

1. Hardware SPI is at least 10 times faster but you can only use specific pins for some signals (look [here](index.html) for which pins).
2. Software SPI is slower (which only mattesrs if you need really tight timings) but you can freely choose which (free) pins you want to use.

### Small introduction to SPI

The Serial Peripheral Interface (SPI) is a synchronous serial communication interface specification used for short-distance communication, primarily in embedded systems.
In this case your microcontroller is the master which controls the slave (at least one 7-Segment display or led matrix).

The master has three significant pins: SCLK(Serial Clock), MOSI(Master Out Slave In), SS(Slave Select).
The slave also has tree needed pins (and two for power(GND and VCC)): CLK(Clock), DIN(Data In), CS(Ship Select).
SCLK has to be connected to CLK and MOSI to DIN.
These signals can be shared by multiple SPI slaves even if they have nothing to do with each other.
SS hat to be connected to CS and each Slave has its own SS pin(which can be any free pin).

## Getting started

To get started you need to first create a new controller_configuration object.
This is used to configure the setup of the LedController which can get complex.
controller_configuration has no constructor and is more like an struct with methods than it is a 'real' class.

First things first, if you want to use Hardware Spi set useHardwareSpi to true, otherwise set it to false.
If you use hardware SPI you only need to specify the CS pin, if not you also need to CLK and MOSI pins.
Just assign the wanted values to SPI_CS, SPI_CLK and SPI_MOSI.

Next you need to specify the total number of segments you have.
Just assign the total number of segments to SegmentCount.
To keep it simple all segments are connected in series, so there is only one row.
To see how you can use more than one row with the LedController go [here](dc/dc4/md_doc_multi_row.html).

Now you can check if the configuration is Valid by calling the isValid() method.
If it returns true, you can continue wit hthe next step, if not try setting debug_output and figure out what went wrong.

Now that you have a valid configuration, you can pass it to the constructor of a new LedController object.
You can use the setRow and setSegment to send data to the LedMatrix.

