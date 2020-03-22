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
#include <type_traits>

#define MAX_SEGMENTS 8

const static byte charTable [] PROGMEM  = {
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B10000000,B00000001,B10000000,B00000000,
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,B00000000,
    B00110111,B00000000,B00000000,B00000000,B00001110,B00000000,B00000000,B00000000,
    B01100111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001000,
    B00000000,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,B00000000,
    B00110111,B00000000,B00000000,B00000000,B00001110,B00000000,B00010101,B00011101,
    B01100111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};

/**
 * @brief This class provied a control interface for MAX7219 and MAX7221 Led display drivers.
 * @long This Controller Class is mainly target at led matracies consisting of more than 1 segment.
    * While it can also handle 7-Segment Displays it is not tested that well.
 * @warning This object is not thread safe yet.
 * 
 * @todo add functions to shift the pixes up, down, left and right
 * @todo rename status to ledstates or something like that
 * @todo make it threading safe
 */
class LedController{
private:

    std::array< std::array<byte,8>, MAX_SEGMENTS> status;

    ///The pin for the data transfer (DIN)
    unsigned int SPI_DIN;

    ///The pin for the chip select signal (CS)
    unsigned int SPI_CS;

    ///The pin for the clock signal (CLK)
    unsigned int SPI_CLK;

    ///The number of connected Segments
    unsigned int SegmentCount;

    ///The mutex for the intensity level, needed to make it threading safe.

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
    std::array<byte,MAX_SEGMENTS*2> spidata;

    /**
     * @brief Set the brightness of the segment.
     * 
     * @param segmentNumber the address of the segment to control
     * @param newIntesityLevel the brightness of the display. (0..15)
     */
    void setIntensity(unsigned int segmentNumber, unsigned int newIntesityLevel);

public:
    /**
     * @brief Construct a new LedController object
     * 
     * @param dataPin pin on the Arduino where data gets shifted out (DIN)
     * @param clkPin pin for the clock (CLK)
     * @param csPin pin for selecting the device (CS)
     * @param numSegments The number of segments that will be controlled by the controller (default 4)
     */
    LedController(unsigned int dataPin, unsigned int clkPin, unsigned int csPin, unsigned int numSegments = 4);

    /**
     * @brief Set the Intensity of the whole matrix to the given value.
     * @note if you want to save more energy disable segments you don't need or lower the brightness.
     * @param newIntesityLevel the new brightness of the matrix. (0..15)
     */
    void setIntensity(unsigned int newIntesityLevel);

    /**
     * @brief Display 8 lines on the given segment
     * 
     * @param segmentindex the Segment number of the desired segment
     * @param data an array containing the data for all the pixels that should be displayed on that segment
     */
    void displayOnSegment(unsigned int segmentindex, byte data[8]);

    /**
     * @brief activates all segments, sets to same intensity and cleas them
     * 
     */
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

    /**
     * @brief Set a single led to a given value
     * 
     * @param segmentNumber the segment number of the desired led
     * @param row the row of the desired led (0..7)
     * @param column the column of the desired led (0..7)
     * @param state true if it should be on otherwise false
     */
    void setLed(unsigned int segmentNumber, unsigned int row, unsigned int column, boolean state);

    /**
     * @brief Set one column of a given segment
     * 
     * @param segmentNumber The desired Segment number
     * @param col The desired column
     * @param value The value, this column should have
     */
    void setColumn(unsigned int segmentNumber, unsigned int col, byte value);

    /**
     * @brief Set a hexadecimal digit on a 7-Segment Display
     * 
     * @param segmentNumber The number of the desired Segment
     * @param digit the position of the digit on the Segment (0..7)
     * @param value the value to be displayed. (0x00..0x0F)
     * @param dp if true sets the decimal point
     */
    void setDigit(unsigned int segmentNumber, unsigned int digit, byte value, boolean dp);
    
    /**
     * @brief Set the Display a character on a 7-Segment display.
     * @note There are only a few characters that make sense here :
         *	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
         *  'A', 'b', 'c', 'd', 'E', 'F', 'H', 'L', 'P', 
         *  '.', '-', '_', ' ' 
     * @param segmentNumber The number of the desired segment
     * @param digit the position of the character on the segment (0..7)
     * @param value the character to be displayed. 
     * @param dp dp if true sets the decimal point
     */
    void setChar(unsigned int segmentNumber, unsigned int digit, char value, boolean dp);

    /**
     * @brief moves the data left by one
     * 
     * @param shiftedInColumn The column that will be shifted in on the right (default 0x00)
     * @return byte The column that gets shifted out on the right
     */
    byte moveLeft(byte shiftedInColumn = 0x00);

    /**
     * @brief refreshes all segments displaying the internally stored state of the segments.
     * 
     */
    void refreshSegments();

    /**
     * @brief Turns an array of rows into an array of columns
     * 
     * @param rowArray the array of rows of which you want the columns
     * @return std::array<byte,8> the columns of the provided row array
     */
    std::array<byte,8> makeColumns(std::array<byte,8> rowArray);
};

