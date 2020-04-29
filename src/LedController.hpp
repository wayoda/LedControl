#pragma once

#ifndef MAX_SEGMENTS
    #define MAX_SEGMENTS 8
#endif 

#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

using C_ByteBlock = byte[8];
using C_Matrix = byte[MAX_SEGMENTS][8];
using C_ByteRow = byte[MAX_SEGMENTS];

#ifdef __has_include

    #define INCLUDED_PGMSPACE
    #if(__has_include(<avr/pgmspace.h>))
        #include <avr/pgmspace.h>
    #else
        #include <pgmspace.h>
    #endif

    #if(__has_include(<array>))
        #include <array>
        #define STD_CAPABLE 1
    #else
        #define STD_CAPABLE 0
    #endif

#endif

#ifndef INCLUDED_PGMSPACE
    #include <avr/pgmspace.h>
    #define INCLUDED_PGMSPACE
#endif

#ifndef STD_CAPABLE
    #define STD_CAPABLE 0
#endif

#if (STD_CAPABLE > 0)

    #include <array>

    #define ByteBlock std::array<byte,8>
    #define Matrix std::array< std::array<byte,8>, MAX_SEGMENTS>
    #define ByteRow std::array<byte,MAX_SEGMENTS>

#else

    using ByteBlock = C_ByteBlock;
    using Matrix = C_Matrix;
    using ByteRow = C_ByteRow;

#endif

#include <SPI.h>

const static byte charTable [] PROGMEM  = {
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
	B00000000,B10110000,B00100010,B00111111,B01011011,B10100101,B00110001,B00000010,
	B01001010,B01101000,B01000010,B00000111,B00000100,B00000001,B10000000,B00100101,
	B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
	B01111111,B01111011,B01001000,B01011000,B01000011,B00001001,B01100001,B11100101,
	B01111101,B01110111,B00011111,B01001110,B00111101,B01001111,B01000111,B01011110,
	B00110111,B00000110,B00111100,B01010111,B00001110,B01010100,B01110110,B01111110,
	B01100111,B01101011,B01100110,B01011011,B00001111,B00111110,B00111110,B00101010,
	B00110111,B00111011,B01101101,B01001110,B00010011,B01111000,B01100010,B00001000,
	B00100000,B01111101,B00011111,B00001101,B00111101,B01101111,B01000111,B01111011,
	B00010111,B00000100,B00011000,B01010111,B00000110,B00010100,B00010101,B00011101,
	B01100111,B01110011,B00000101,B01011011,B00001111,B00011100,B00011100,B00010100,
	B00110111,B00111011,B01101101,B00110001,B00000110,B00000111,B01000000,B00000000
};

/**
 * @brief This class provied a control interface for MAX7219 and MAX7221 Led display drivers.
 * @details This Controller Class is mainly target at led matracies consisting of more than 1 segment.
    * While it can also handle 7-Segment Displays it is not tested that well.
 * @warning This object is not thread safe yet.
 * @note If your compiler is STD_CAPABLE the ByteBlock is of the type std::array<byte,8> instead of C_ByteBlock.
 * @note Check if your code has support for the std by calling: static_assert(STD_CAPABLE > 0, "STD not supported")
 * 
 * @todo make it threading safe
 */
class LedController{
private:

    Matrix LedStates;

    ///The pin for the data transfer (DIN)
    unsigned int SPI_MOSI;

    ///The pin for the chip select signal (CS)
    unsigned int SPI_CS;

    ///The pin for the clock signal (CLK)
    unsigned int SPI_CLK;

    ///The number of connected Segments
    unsigned int SegmentCount;

    ///True if hardware spi should be use (a lot faster  but you cannot use any pin you want)
    bool useHardwareSpi = false;

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
     * @param opcode The command thCLKat should be executed
     * @param data The data needed for that command
     */
    void spiTransfer(unsigned int segment, byte opcode, byte data);

    ///The array for shifting the data to the devices
    byte spidata[MAX_SEGMENTS*2];

    /**
     * @brief Set the brightness of the segment.
     * 
     * @param segmentNumber the address of the segment to control
     * @param newIntesityLevel the brightness of the display. (0..15)
     */
    void setIntensity(unsigned int segmentNumber, unsigned int newIntesityLevel);

    ByteRow emptyRow;

    int createEmptyRow(C_ByteRow* row);

public:

    /**
     * @brief Construct a new LedController for use with hardware SPI
     * 
     * @param csPin The pin to select the led matrix
     * @param numSegments the number of connected segments (defualt 4)
     */
    LedController(unsigned int csPin, unsigned int numSegments = 4);

    /**
     * @brief Construct a new LedController object
     * 
     * @param dataPin pin on the Arduino where data gets shifted out (DIN)
     * @param clkPin pin for the clock (CLK)
     * @param csPin pin for selecting the device (CS)
     * @param numSegments The number of segments that will be controlled by the controller (default 4)
     * @param useHardwareSpi true if you want to use hardware SPI (view https://www.arduino.cc/en/Reference/SPI for pin config)
     */
    LedController(unsigned int dataPin, unsigned int clkPin, unsigned int csPin, unsigned int numSegments = 4, bool useHardwareSpi = false);

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
    void displayOnSegment(unsigned int segmentindex, ByteBlock data);

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
     * @brief refreshes all segments displaying the internally stored state of the segments.
     * 
     */
    void refreshSegments();

    /**
     * @brief moves the data left by one
     * 
     * @param shiftedInColumn The column that will be shifted in on the right (default 0x00)
     * @return byte The column that gets shifted out on the left
     */
    byte moveLeft(byte shiftedInColumn = 0x00);


    /**
     * @brief moves the data left by one
     * 
     * @param shiftedInColumn The column that will be shifted in on the left (default 0x00)
     * @return byte The column that gets shifted out on the right
     */
    byte moveRight(byte shiftedInColumn = 0x00);

    /**
     * @brief This function changes to bitorder of a byte (useful to mirror "images" you want to display)
     * 
     * @param input The byte that should be reversed
     * @return byte The reversed byte
     */
    byte reverse(byte input);

    /**
     * @brief moves the data up by one
     * 
     * @param shiftedInRow The row that will be shifted in on the bottom (default 0x00)
     * @param shiftedOutRow The address of the row that will be shifted out on the bottom 
     */
    void moveUp(C_ByteRow shiftedInRow, C_ByteRow* shiftedOutRow);

    /**
     * @brief moves the data down by one
     * 
     * @param shiftedInRow The row that will be shifted in on the top (default 0x00)
     * @param shiftedOutRow The address of the row that will be shifted out on the bottom
     */
    void moveDown(C_ByteRow shiftedInRow, C_ByteRow* shiftedOutRow);

    /**
     * @brief moves the data up by oneand 0x00 will be shifted in
     * 
     * @param shiftedOutRow The address of the row that will be shifted out on the bottom
     */
    void moveUp(C_ByteRow* shiftedOutRow);

    /**
     * @brief moves the data down by one and 0x00 will be shifted in
     * 
     * @param shiftedOutRow The address of the row that will be shifted out on the bottom
     */
    void moveDown(C_ByteRow* shiftedOutRow);

    /**
     * @brief moves the data up by one and 0x00 will be shifted in
     * 
     */
    void moveUp();

    /**
     * @brief moves the data down by one and 0x00 will be shifted in
     * 
     * @return ByteRow The row the will be shifted out on the bottom
     */
    void moveDown();

    /**
     * @brief Turns an array of rows into an array of columns
     * 
     * @param rowArray the array of rows of which you want the columns
     * @param columnArray The address where the result will be stored
     */
    void makeColumns(C_ByteBlock rowArray, C_ByteBlock* columnArray);

    /**
     * @brief Reverse an array of 8 bytes (mirror it)
     * 
     * @param input The array that should be mirrored
     * @param reversedInput The address where the result will be stored
     */
    void reverse(C_ByteBlock input, C_ByteBlock* reversedInput);

    /**
     * @brief rotate an byte[8] array by 180 degrees
     * 
     * @param input the array that will be rotated
     * @param rotatedInput The address where the result will be stored
     */
    void rotate180(C_ByteBlock input, C_ByteBlock* rotatedInput);

    #if (STD_CAPABLE > 0)

        /**
         * @brief moves the data up by one
         * @warning call static_assert(STD_CAPABLE > 0, "STD not supported") before calling this function
         * 
         * @param shiftedInRow The row that will be shifted in on the bottom 
         * @return ByteRow The row the will be shifted out on the top
         */
        ByteRow moveUp(ByteRow shiftedInRow);

        /**
         * @brief moves the data down by one
         * @warning call static_assert(STD_CAPABLE > 0, "STD not supported") before calling this function
         * 
         * @param shiftedInRow The row that will be shifted in on the top (default 0x00)
         * @return ByteRow The row the will be shifted out on the bottom
         */
        ByteRow moveDown(ByteRow shiftedInRow);

        /**
         * @brief Turns an array of rows into an array of columns
         * @warning call static_assert(STD_CAPABLE > 0, "STD not supported") before calling this function
         * 
         * @param rowArray the array of rows of which you want the columns
         * @return ByteBlock the columns of the provided row array
         */
        ByteBlock makeColumns(ByteBlock rowArray);

        /**
         * @brief Reverse an array of 8 bytes (mirror it)
         * @warning call static_assert(STD_CAPABLE > 0, "STD not supported") before calling this function
         * 
         * @param input The array that should be mirrored
         * @return ByteBlock The mirrored array
         */
        ByteBlock reverse(ByteBlock input);

        /**
         * @brief rotate an byte[8] array by 180 degrees
         * @warning call static_assert(STD_CAPABLE > 0, "STD not supported") before calling this function
         * 
         * @param input the array that will be rotated
         * @return ByteBlock The rotated array
         */
        ByteBlock rotate180(ByteBlock input);

    #endif
};

