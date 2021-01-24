# How to use this library

**Check the LedController for more infomation about its functions or look for the [Examples](examples.html).**

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
Because it is a template you need to know the dimension of your Matrix which causes the type to be `controller_configuration<sements_x,segments_y>`.
To see how you can use more than one row with the LedController go [here](@ref multi_row).
The rest of the example will use `controller_configuration<4,1>` like in the [rocket example](@ref LedControllerDemoRocket.ino).

If you want to use Hardware Spi set useHardwareSpi to true, otherwise set it to false.
If you use hardware SPI you only need to specify the CS pin, if not you also need to CLK and MOSI pins.
Just assign the wanted values to SPI_CS, SPI_CLK and SPI_MOSI.

Now you can check if the configuration is Valid by calling the isValid() method.
If it returns true, you can continue with the next step, if not try setting debug_output and figure out what went wrong.

Now that you have a valid configuration, you can pass it to the init function of a LedController object.
You can use the setRow and setSegment to send data to the LedMatrix.

setRow sets a specific Row of a given segment to a given byte.
It needs the segment index which can be calculated from the segment coordinates by calling `lc.getConfig().getSegmentNumber(x,y)`.

setSegment sets a whole segment but the segment can be specified through coordinates or the index.

The following snippet is (probably working) sample code that initilizes a LedController using Hardware spi and sets some segments using setRow and setSegment.

```cpp

#include "LedController.hpp"
auto lc = LedController<4,1>();

void setup(){
    auto conf = controller_configuration<4,1>();
    conf.useHardwareSpi = true;
    conf.SPI_CS = 25;

    lc.init(conf);

    lc.setRow(lc.getConfig().getSegmentNumber(0,1),2,0xAA);
    lc.setSegment(lc.getConfig().getSegmentNumber(1,1),{0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F});
    lc.setSegment(2,1,{0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x01,0x00});

}

void loop(){

}

```

## What is next?

Now that you know how to use the LedController you can check the more advanced features like [using multiple rows](@ref multi_row) or [moving the displayed data](movement.md).
