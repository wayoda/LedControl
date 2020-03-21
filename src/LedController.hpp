#pragma once

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
#include <vector>
#include <array>

#define MAX_SEGMENTS 8

class LedController{
private:

    std::vector< std::vector<byte> > status;

    ///The pin for the data transfer (DIN)
    unsigned int SPI_DIN;

    ///The pin for the chip select signal (CS)
    unsigned int SPI_CS;

    ///The pin for the clock signal (CLK)
    unsigned int SPI_CLK;

    ///The number of connected Segments
    unsigned int SegmentCount;

    ///The mutex for the intensity level, needed to make it threading safe.
    std::mutex mut_IntensityLevel;

    /**
     * @brief The current brightness level of the leds.
     * @warning The value needs to be between 0 and 15 (inclusive bounds).
     */
    unsigned int IntensityLevel = 1;

    /**
     * @brief This function transfers one command to the attached module
     * 
     * @param segment The segment that should execute this command 
     * @param opcode The command that should be executed
     * @param data The data needed for that command
     */
    void spiTransfer(unsigned int segment, byte opcode, byte data);

    ///The array for shifting the data to the devices
    std::vector<byte> spidata;

    /**
     * @brief Set the brightness of the segment.
     * 
     * @param segmentNumber the address of the segment to control
     * @param newIntesityLevel the brightness of the display. (0..15)
     */
    void setIntensity(unsigned int segmentNumber, unsigned int newIntesityLevel);

public:
    LedController();
    LedController(unsigned int dataPin, unsigned int clkPin, unsigned int csPin, unsigned int numDevices);
    ~LedController();

    /**
     * @brief Set the Intensity of the whole matrix to the given value.
     * @note if you want to save more energy disable segments you don't need or lower the brightness.
     * @param newIntesityLevel the new brightness of the matrix. (0..15)
     */
    void setIntensity(unsigned int newIntesityLevel);

    void displayOnSegment(unsigned int segmentindex, byte data[8]);

    std::shared_ptr<LedControl> getLedControl();
    void resetMatrix();

    /**
     * @brief clears all segments, turning all LEDs off.
     * 
     */
    void clearMatrix();

    /**
     * @brief Get the number of configured segments
     * 
     * @return unsigned int The number of configured segments
     */
    unsigned int getSegmentCount();
    
    /**
     * @brief Set the segment in power-down mode.
     * 
     * @param segmentNumber The segment to control
     */
    void shutdownSegment(unsigned int segmentNumber);

    /**
     * @brief Get the segment out of power-down mode for normal operation.
     * 
     * @param segmentNumber The segment to control
     */
    void activateSegment(unsigned int segmentNumber);

    /**
     * @brief Set all segments into power-down mode
     * 
     */
    void shutdownAllSegments();

    /**
     * @brief Get all segments out of power-down mode for normal operation.
     * 
     */
    void activateAllSegments();
    
    /**
     * @brief Set the number of digits (or rows) to be displayed.
     * @note See datasheet for sideeffects of the scanlimit on the brightness of the display.
     * @param segmentNumber The segment which should be addressed
     * @param limit The number of digits to be displayed (0..7)
     */
    void setScanLimit(unsigned int segmentNumber, unsigned int limit);

    /**
     * @brief clears a given segment, turning all its LEDs off.
     * 
     * @param segmentNumber The segment to control.
     */
    void clearSegment(unsigned int segmentNumber);

    /**
     * @brief Set one Row of one segment.
     * 
     * @param segmentNumber The Segment which should be modified 
     * @param row The row which should be modified 
     * @param value each bit set to 1 will light up the corresponding Led.
     */
    void setRow(unsigned int segmentNumber, unsigned int row, byte value);
};

