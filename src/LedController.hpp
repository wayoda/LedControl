#include "LedControl.h"

#ifndef LedControl_h
#define LedControl_h

#if (defined(AVR))
#include <avr\pgmspace.h>
#else
#include <pgmspace.h>
#endif

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <mutex>
#include <thread>
#include <memory>

class LedController{
private:

    ///The pin for the data transfer (DIN)
    unsigned int Data;

    ///The pin for the chip select signal (CS)
    unsigned int ChipSelect;

    ///The pin for the clock signal (CLK)
    unsigned int Clock;

    ///The number of connected Segments
    unsigned int SegmentCount;

    ///The mutex for the intensity level, needed to make it threading safe.
    std::mutex mut_IntensityLevel;

    /**
     * @brief The current brightness level of the leds.
     * @warning The value needs to be between 0 and 15 (inclusive bounds).
     */
    unsigned int IntensityLevel = 1;

    std::shared_ptr<LedControl> lc;

    /**
     * @brief This function transfers one command to the attached module
     * 
     * @param segment The segment that should execute this command 
     * @param opcode The command that should be executed
     * @param data The data needed for that command
     */
    void spiTransfer(unsigned int segment, byte opcode, byte data);

    ///The array for shifting the data to the devices
    byte spidata[16];

public:
    LedController();
    LedController(unsigned int dataPin, unsigned int clkPin, unsigned int csPin, unsigned int numDevices);
    ~LedController();

    void setIntensity(unsigned int newIntesityLevel);
    void displayOnSegment(unsigned int segmentindex, byte data[8]);

    std::shared_ptr<LedControl> getLedControl();
    void resetMatrix();
    void clearMatrix();

    unsigned int getSegmentCount();

    void shutdownSegment(unsigned int segmentNumber);
    void activateSegment(unsigned int segmentNumber);

    void shutdownAllSegments();
    void activateAllSegments();
};

