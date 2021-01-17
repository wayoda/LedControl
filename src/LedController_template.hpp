#pragma once

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#if __has_include("ArduinoFake.h")
#include "ArduinoFake.h"
#else
#include <WProgram.h>
#endif
#endif

#include <LedController_byteblock.hpp>

#ifdef __has_include

#define INCLUDED_PGMSPACE
#if (__has_include(<avr/pgmspace.h>))
#include <avr/pgmspace.h>
#else
#if (__has_include(<pgmspace.h>))
#define INCLUDED_PGMSPACE
#include <pgmspace.h>
#else
#define INCLUDED_PGMSPACE
#ifndef pgm_read_byte
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_byte_near
#define pgm_read_byte_near(addr)  pgm_read_byte(addr)
#endif
#endif
#endif

#endif

#ifndef INCLUDED_PGMSPACE
#include <pgmspace.h>
#define INCLUDED_PGMSPACE
#endif

#include <SPI.h>

#include <LedController_config.hpp>

/**
 * \~
 * @todo make it threading safe
 * 
 * \~english
 * @brief This class provied a control interface for MAX7219 and MAX7221 Led
 * display drivers.
 * @details This Controller Class is mainly target at led matracies consisting
 * of more than 1 segment. While it can also handle 7-Segment Displays it is not
 * tested that well.
 * @warning This object is not thread safe yet.
 * 
 * \~german
 * @brief  Diese Klasse bietet ein Interface zur Steuerung von MAX7219 uns MAX7221 LED
 * Displaytreibern.
 * @details Diese Steuerundsklasse fokusiert sich primär auf Led Matrizen bestehend
 * aus mehr als einem Segment. Auch wenn sie 7-Segmentanzeigen steuern kann ist dies nicht getestet.
 * @warning Diese Klasses ist nicht therding-sicher.
 *
 */
template <size_t columns, size_t rows>
class LedController {
protected:
  /// The state of all the Leds
  ByteBlock LedStates[columns*rows];

  /**
   * 
   * \~english
   * @brief The configuration of the LedController
   *
   */
  controller_configuration<columns,rows> conf;

  /**
   * \~english
   * @brief This function transfers one command to the attached module
   *
   * @param segment The segment that should execute this command
   * @param opcode The command thCLKat should be executed
   * @param data The data needed for that command
   */
  void spiTransfer(unsigned int segment, byte opcode, byte data);
  
  /**
   * \~english
   * @brief The array for shifting the data to the devices
   * 
   */
  byte spidata[rows][columns * 2];

  /**
   * \~english
   * @brief Set the brightness of the segment.
   *
   * @param segmentNumber the address of the segment to control
   * @param newIntesityLevel the brightness of the display. (0..15)
   */
  void setIntensity(unsigned int segmentNumber, unsigned int newIntesityLevel);

  /**
   * \~english
   * @brief True if the LedController is fully initilized
   *
   */
  bool initilized = false;

  /**
   * \~english
   * @brief initilize the internal buffers of the Controller.
   * 
   */
  void resetBuffers();

  /**
   * \~english
   * @brief initilize the spi outputs
   * 
   */
  void initSPI();

  /**
   * \~english
   * @brief initilize the configuration
   * 
   */
  void initConf();

public:
  /**
   * \~english
   * @brief Construct a new LedController without initilizing anything.
   *
   */
  LedController();

  /**
   * \~english
   * @brief Construct a new LedController for use with hardware SPI
   *
   * @param csPin The pin to select the led matrix
   */
  LedController(unsigned int csPin);

  /**
   * \~english
   * @brief Construct a new LedController object
   *
   * @param dataPin pin on the Arduino where data gets shifted out (DIN)
   * @param clkPin pin for the clock (CLK)
   * @param csPin pin for selecting the device (CS)
   * @param useHardwareSpi true if you want to use hardware SPI (view
   * https://www.arduino.cc/en/Reference/SPI for pin config)
   */
  LedController(unsigned int dataPin, unsigned int clkPin, unsigned int csPin,
                bool useHardwareSpi = false);

  /**
   * \~english
   * @brief Construct a new Led Controller from a given configuration
   *
   * @param configuration the configuration that should be used for the
   * Controller
   */
  LedController(const controller_configuration<columns,rows> &configuration);

  /**
   * \~english
   * @brief Destroy the Led Controller object and free the memory
   *
   */
  ~LedController();

  /**
   * \~english
   * @brief The copy constructor for the LedController
   *
   * @param other the LedController which should have its state copied
   */
  LedController(const LedController &other);

  /**
   * \~english
   * @brief initilizes the LedController for use with hardware SPI
   *
   * @param csPin The pin to select the led matrix
   */
  void init(unsigned int csPin);

  /**
   * \~english
   * @brief initilizes the LedController
   *
   * @param dataPin pin on the Arduino where data gets shifted out (DIN)
   * @param clkPin pin for the clock (CLK)
   * @param csPin pin for selecting the device (CS)
   * @param useHardwareSpi true if you want to use hardware SPI (view
   * https://www.arduino.cc/en/Reference/SPI for pin config)
   */
  void init(unsigned int dataPin, unsigned int clkPin, unsigned int csPin,
            bool useHardwareSpi = false);

  /**
   * \~english
   * @brief initilizes the LedController wit ha given configuration
   *
   * @param configuration
   */
  void init(const controller_configuration<columns,rows> &configuration);

  /**
   * \~english
   * @brief returns the status of the LedController
   *
   * @return true the LedController is initilized
   * @return false the LedController is not initilized
   */
  bool isInitilized();

  /**
   * \~english
   * @brief Set the Intensity of the whole matrix to the given value.
   * @note if you want to save more energy disable segments you don't need or
   * lower the brightness.
   * @param newIntesityLevel the new brightness of the matrix. (0..15)
   */
  void setIntensity(unsigned int newIntesityLevel);

  /**
   * \~english
   * @brief Display 8 lines on the given segment
   *
   * @param segmentindex the Segment number of the desired segment
   * @param data an array containing the data for all the pixels that should be
   * displayed on that segment
   */
  void displayOnSegment(unsigned int segmentindex, ByteBlock data);

  /**
   * \~english
   * @brief  Display 8 lines on the given segment
   * 
   * @param column the column where the wanted segment is
   * @param row_num the row where the wanted segment is
   * @param data an array containing the data for all the pixels that should be
   * displayed on that segment
   */
  void displayOnSegment(unsigned int column, unsigned int row_num, ByteBlock data);

  /**
   * \~english
   * @brief Get the Segment Data of a specific Segment
   * 
   * @param column the column where the wanted segment is
   * @param row_num the row where the wanted segment is
   * @return ByteBlock the requested segment
   */
  ByteBlock getSegmentData(unsigned int column, unsigned int row_num);

  /**
   * \~english
   * @brief Get the Segment Data of a specific Segment
   * 
   * @param segmentindex the index of whose data you want to have
   * @return ByteBlock the requested segment
   */
  ByteBlock getSegmentData(unsigned int segmentindex);

  /**
   * \~english
   * @brief activates all segments, sets to same intensity and cleas them
   *
   */
  void resetMatrix();

  /**
   * \~english
   * @brief clears all segments, turning all LEDs off.
   *
   */
  void clearMatrix();

  /**
   * \~english
   * @brief Get the number of configured segments
   *
   * @return unsigned int The number of configured segments
   */
  unsigned int getSegmentCount();

  /**
   * \~english
   * @brief Set the segment in power-down mode.
   *
   * @param segmentNumber The segment to control
   */
  void shutdownSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief Get the segment out of power-down mode for normal operation.
   *
   * @param segmentNumber The segment to control
   */
  void activateSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief Set all segments into power-down mode
   *
   */
  void shutdownAllSegments();

  /**
   * \~english
   * @brief Get all segments out of power-down mode for normal operation.
   *
   */
  void activateAllSegments();

  /**
   * \~english
   * @brief Set the number of digits (or rows) to be displayed.
   * @note See datasheet for sideeffects of the scanlimit on the brightness of
   * the display.
   * @param segmentNumber The segment which should be addressed
   * @param limit The number of digits to be displayed (0..7)
   */
  void setScanLimit(unsigned int segmentNumber, unsigned int limit);

  /**
   * \~english
   * @brief clears a given segment, turning all its LEDs off.
   *
   * @param segmentNumber The segment to control.
   */
  void clearSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief Set one Row of one segment.
   *
   * @param segmentNumber The Segment which should be modified
   * @param row The row which should be modified
   * @param value each bit set to 1 will light up the corresponding Led.
   */
  void setRow(unsigned int segmentNumber, unsigned int row, byte value);

  /**
   * \~english
   * @brief get one Row of one segment.
   *
   * @param segmentNumber The Segment which should be modified
   * @param row The row which should be modified
   * @return value each bit set to 1 will light up the corresponding Led.
   */
  byte getRow(unsigned int segmentNumber, unsigned int row);

  /**
   * \~english
   * @brief Set a single led to a given value
   *
   * @param segmentNumber the segment number of the desired led
   * @param row the row of the desired led (0..7)
   * @param column the column of the desired led (0..7)
   * @param state true if it should be on otherwise false
   */
  void setLed(unsigned int segmentNumber, unsigned int row, unsigned int column,
              boolean state);

  /**
   * \~english
   * @brief Set one column of a given segment
   *
   * @param segmentNumber The desired Segment number
   * @param col The desired column
   * @param value The value, this column should have
   */
  void setColumn(unsigned int segmentNumber, unsigned int col, byte value);

  /**
   * \~english
   * @brief Set a hexadecimal digit on a 7-Segment Display
   *
   * @param segmentNumber The number of the desired Segment
   * @param digit the position of the digit on the Segment (0..7)
   * @param value the value to be displayed. (0x00..0x0F)
   * @param dp if true sets the decimal point
   */
  void setDigit(unsigned int segmentNumber, unsigned int digit, byte value,
                boolean dp);

  /**
   * \~english
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
  void setChar(unsigned int segmentNumber, unsigned int digit, char value,
               boolean dp);

  /**
   * \~english
   * @brief refreshes all segments by first resetting them and then updating
   * them.
   *
   */
  void refreshSegments();

  /**
   * \~english
   * @brief refreshes a given segment by first resetting it and then updating
   * it.
   *
   * @param segmentNumber the segment that will be resetted
   */
  void refreshSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief update all segments by displaying the internally stored state of the
   * segments.
   *
   */
  void updateSegments();

  /**
   * \~english
   * @brief update a given segment by displaying the internally stored state of
   * the segment.
   *
   * @param segmentNumber the segment that will be updated
   */
  void updateSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief This function changes to bitorder of a byte (useful to mirror
   * "images" you want to display)
   *
   * @param input The byte that should be reversed
   * @return byte The reversed byte
   */
  byte reverse(byte input);

  /**
   * \~english
   * @brief moves the data up by one and 0x00 will be shifted in
   *
   */
  void moveUp();

  /**
   * \~english
   * @brief moves the data down by one and 0x00 will be shifted in
   *
   */
  void moveDown();

  /**
   * \~english
   * @brief moves the data left by one and 0x00 will be shifted in
   *
   */
  void moveLeft();

  /**
   * \~english
   * @brief moves the data right by one and 0x00 will be shifted in
   *
   */
  void moveRight();

  /**
   * \~english
   * @brief moves the data left by one
   *
   * @param shiftedInColumn The column that will be shifted to the right
   * (default 0x00)
   * @param row_num The row that will be shifted to the right
   * @warning ONLY moves one row
   * @return byte The column that gets shifted out on the left
   */
  byte moveRowLeft(byte shiftedInColumn = 0x00, unsigned int row_num = 0);

  /**
   * \~english
   * @brief moves the data left by one
   *
   * @param shiftedInColumn The column that will be shifted to the left
   * (default 0x00)
   * @param row_num The row that will be shifted to the left
   * @warning ONLY moves one row
   * @return byte The column that gets shifted out on the right
   */
  byte moveRowRight(byte shiftedInColumn = 0x00, unsigned int row_num = 0);

  /**
   * \~english
   * @brief moves the data of a column up by one
   * 
   * @param shiftedInRow the date the will be shifted in on the bottom
   * @param col_num the index of the column that will be moved
   * @return byte the row that will be shifted out
   */
  byte moveColumnUp(byte shiftedInRow = 0x00, unsigned int col_num = 0);

  /**
   * \~english
   * @brief moves the data of a column down by one
   * 
   * @param shiftedInRow the date the will be shifted in on the top
   * @param col_num the index of the column that will be moved
   * @return byte the row that will be shifted out
   */
  byte moveColumnDown(byte shiftedInRow = 0x00, unsigned int col_num = 0);

  /**
   * \~english
   * @brief Turns an ByteBlock of rows into an ByteBlock of columns
   * 
   * @param rowArray the ByteBlock of rows of which you want the columns of
   * @return ByteBlock The Columns of the given ByteBlock
   */
  ByteBlock makeColumns(ByteBlock rowArray);

  /**
   * \~english
   * @brief Reverse an ByteBlock of 8 bytes (mirror it)
   * 
   * @param input The ByteBlock that should be mirrored
   * @return ByteBlock The reversed ByteBlock
   */
  ByteBlock reverse(ByteBlock input);

  /**
   * \~english
   * @brief rotate an ByteBlock by 180 degrees
   * 
   * @param input the ByteBlock that will be rotated
   * @return ByteBlock The rotated ByteBlock
   */
  ByteBlock rotate180(ByteBlock input);

  /**
   * \~english
   * @brief Get the Config of the Led Controller
   *
   * @return controlller_configuration the configuration
   */
  const controller_configuration<columns,rows>& getConfig();

  //The following methods are deprecated and will be removed in the future
  //They only exist to help the transition to a new version

  ///@todo remove following functions in version 2.1.0

  /**
   * \~english
   * @brief moves the data left by one
   * @deprecated to be reomoved in version 2.1.0, moveRowLeft should be used
   * @param shiftedInColumn The column that will be shifted to the right
   * (default 0x00)
   * @warning ONLY moves row 0, this function exists for backwards compatibility
   * @return byte The column that gets shifted out on the left
   */
  byte moveLeft(byte shiftedInColumn);

  /**
   * \~english
   * @brief moves the data left by one
   * @deprecated to be reomoved in version 2.1.0, moveRowRight should be used
   * @param shiftedInColumn The column that will be shifted to the left
   * (default 0x00)
   * @warning ONLY moves row 0, this function exists for backwards compatibility
   * @return byte The column that gets shifted out on the right
   */
  byte moveRight(byte shiftedInColumn);

  /**
   * \~english
   * @brief moves all rows to the left.
   * The passed Arrays need to have the same length as the number of rows, or be a nullptr.
   * If shiftedInColumn is a nullptr, 0x00 will be used for all rows.
   * 
   * @param shiftedInColumn This Array contains what will be shifted in on each Row and needs to be the same size as number of rows or nullptr.
   * @return ByteRow<rows> This pointer to an Array will contain the bytes that will be shifted out on each Row, it should be the same size as the number of rows or nullptr.
   */
  ByteRow<rows> moveLeft(const ByteRow<rows>& shiftedInColumn);

  /**
   * \~english
   * @brief moves all rows to the right.
   * The passed Arrays need to have the same length as the number of rows, or be a nullptr.
   * If shiftedInColumn is a nullptr, 0x00 will be used for all rows.
   * 
   * @param shiftedInColumn This Array contains what will be shifted in on each Row and needs to be the same size as number of rows or nullptr.
   * @return ByteRow<rows> This pointer to an Array will contain the bytes that will be shifted out on each Row, it should be the same size as the number of rows or nullptr.
   */
  ByteRow<rows> moveRight(const ByteRow<rows>& shiftedInColumn);

  /**
   * \~english
   * @brief moves all columns up.
   * 
   * @param shiftedInColumn This Array contains what will be shifted in on each Row and needs to be the same size as number of rows or nullptr.
   * @return ByteRow<columns> This pointer to an Array will contain the bytes that will be shifted out on each Row, it should be the same size as the number of rows or nullptr.
   */
  ByteRow<columns> moveUp(const ByteRow<columns>& shiftedInColumn);

  /**
   * \~english
   * @brief moves all columns down.
   * 
   * @param shiftedInColumn This Array contains what will be shifted in on each Row and needs to be the same size as number of rows or nullptr.
   * @return ByteRow<columns> This pointer to an Array will contain the bytes that will be shifted out on each Row, it should be the same size as the number of rows or nullptr.
   */
  ByteRow<columns> moveDown(const ByteRow<columns>& shiftedInColumn);


  ///@todo remove following functions in version 2.2.0

  /**
   * \~english
   * @brief Get the Segment Data of a specific Segment
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param column the column where the wanted segment is
   * @param row_num the row where the wanted segment is
   * @param resultLocation the location where the data should be stored
   */
  void getSegmentData(unsigned int column, unsigned int row_num, ByteBlock* resultLocation);

  /**
   * \~english
   * @brief Get the Segment Data of a specific Segment
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param segmentindex the index of whose data you want to have
   * @param resultLocation the location where the data should be stored
   */
  void getSegmentData(unsigned int segmentindex, ByteBlock *resultLocation);

  /**
   * \~english
   * @brief Turns an array of rows into an array of columns
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param rowArray the array of rows of which you want the columns
   * @param columnArray The address where the result will be stored
   */
  void makeColumns(ByteBlock rowArray, ByteBlock *columnArray);

  /**
   * \~english
   * @brief Reverse an array of 8 bytes (mirror it)
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param input The array that should be mirrored
   * @param reversedInput The address where the result will be stored
   */
  void reverse(ByteBlock input, ByteBlock *reversedInput);

  /**
   * \~english
   * @brief rotate an byte[8] array by 180 degrees
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param input the array that will be rotated
   * @param rotatedInput The address where the result will be stored
   */
  void rotate180(ByteBlock input, ByteBlock *rotatedInput);

  /**
   * \~english
   * @brief moves the data up by one
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param shiftedInRow The row that will be shifted in on the bottom (default
   * 0x00)
   * @param shiftedOutRow The address of the row that will be shifted out on the
   * bottom
   */
  void moveUp(const ByteRow<columns>& shiftedInRow, ByteRow<columns>* shiftedOutRow);

  /**
   * \~english
   * @brief moves the data down by one
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param shiftedInRow The row that will be shifted in on the top (default
   * 0x00)
   * @param shiftedOutRow The address of the row that will be shifted out on the
   * bottom
   */
  void moveDown(const ByteRow<columns>& shiftedInRow, ByteRow<columns>* shiftedOutRow);

  /**
   * \~english
   * @brief moves the data up by oneand 0x00 will be shifted in
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param shiftedOutRow The address of the row that will be shifted out on the
   * bottom
   */
  void moveUp(ByteRow<columns>* shiftedOutRow);

  /**
   * \~english
   * @brief moves the data down by one and 0x00 will be shifted in
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param shiftedOutRow The address of the row that will be shifted out on the
   * bottom
   */
  void moveDown(ByteRow<columns>* shiftedOutRow);

  /**
   * \~english
   * @brief moves all rows to the left.
   * The passed Arrays need to have the same length as the number of rows, or be a nullptr.
   * If shiftedInColumn is a nullptr, 0x00 will be used for all rows.
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param shiftedInColumn This Array contains what will be shifted in on each Row and needs to be the same size as number of rows or nullptr.
   * @param shiftedOutColumn This pointer to an Array will contain the bytes that will be shifted out on each Row, it should be the same size as the number of rows or nullptr.
   */
  void moveLeft(const ByteRow<rows>& shiftedInColumn, ByteRow<rows>* shiftedOutColumn);

  /**
   * \~english
   * @brief moves all rows to the right.
   * The passed Arrays need to have the same length as the number of rows, or be a nullptr.
   * If shiftedInColumn is a nullptr, 0x00 will be used for all rows.
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @param shiftedInColumn This Array contains what will be shifted in on each Row and needs to be the same size as number of rows or nullptr.
   * @param shiftedOutColumn This pointer to an Array will contain the bytes that will be shifted out on each Row, it should be the same size as the number of rows or nullptr.
   */
  void moveRight(const ByteRow<rows>& shiftedInColumn, ByteRow<rows>* shiftedOutColumn);
};

/**
 * @example LedControllerDemoRocketMulti.ino
 * An example on how to use the ledcontroller with multiple rows
 */

/**
 * @example LedControllerDemoRocket.ino
 * An example on how to use the ledcontroller to move a rocket across a led matrix
 */

/**
 * @example LedControllerDemoHwSPI.ino
 * This is simply LedControllerDemoRocket.ino just with hardware spi.
 */

/**
 * @example LedControllerDemoCounting.ino
 * An example on how to use the ledcontroller to create a counter.
 */

/**
 * @example LCDemo7Segment.ino
 * An example on how to use the ledcontroller with 7-Segment displays.
 */
