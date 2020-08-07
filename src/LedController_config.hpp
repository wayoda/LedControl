#pragma once

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/**
 * @brief This class is ued to handle the configuration of the LedController
 *
 */
class controller_configuration {
public:
  /**
   * @brief The pin for the data transfer (MOSI on board and DIN on Matrix)
   * @warning if this is 0 and useHardwareSpi is false the initilization will
   * fail
   */
  unsigned int SPI_MOSI = 0;

  /**
   * @brief The pin for the chip select signal (CS).
   * @warning this has to be set if it is 0 the initlization will fail.
   */
  unsigned int SPI_CS = 0;

  /**
   * @brief The pin for the clock signal (CLK)
   * @warning if this is 0 and useHardwareSpi is false the initilization will
   * fail
   */
  unsigned int SPI_CLK = 0;

  /**
   * @brief The total number of connected Segments.
   * @warning while in theory it can be any number be careful not to run out of
   * memory.
   *
   */
  unsigned int SegmentCount = 4;

  /**
   * @brief true if you want to use hardware SPI (view
   * https://www.arduino.cc/en/Reference/SPI for pin config). While this is a
   * lot faster you cannot use every pin for the MOSI and CLK signal. SPI_MOSI
   * and SPI_CLK will be set automatically if this is true.
   */
  bool useHardwareSpi = false;

  /**
   * @brief The current brightness level of the leds.
   * @warning The value needs to be between 0 and 15 (inclusive bounds).
   */
  unsigned int IntensityLevel = 1;

  /**
   * @brief The number of rows the Matrix should have.
   * @warning Currently not in use, will be used in v2.0.0
   * @note The matrix has to be a rectangle if this is not the case the number
   * of rows will be set to 1.
   *
   */
  unsigned int rows = 1;

  /**
   * @brief This Arrays specifies which Pin to use for each row if nullptr
   * SPI_CS will be used.
   * @note If this array is not a nullptr it is assumed it is the same length as
   * the number of rows.
   * @warning Currently not in use, will be used in v2.0.0
   *
   */
  unsigned int *row_SPI_CS = nullptr;

  static bool isValidConfig(const controller_configuration &conf) {
    // checking the clk amd mosi pins
    if (!conf.useHardwareSpi) {
      if (conf.SPI_CLK == 0) {
        Serial.println(
            "No CLK Pin given. Specify one or set useHardwareSpi to true");
        return false;
      }

      if (conf.SPI_MOSI == 0) {
        Serial.println(
            "No MOSI Pin given. Specify one or set useHardwareSpi to true");
        return false;
      }
    }

    // checking the cs pin(s)
    if (conf.SPI_CS == 0 && conf.row_SPI_CS == nullptr) {
      Serial.println("No CS Pin given");
      return false;
    }

    if (conf.row_SPI_CS != nullptr &&
        sizeof(conf.row_SPI_CS) != sizeof(unsigned int) * conf.rows) {
      Serial.println("Wrong row_SPI_CS size, it does not match conf.rows");

      if (conf.SPI_CS != 0) {
        Serial.println("Falling back to SPI_CS for every row (assuming all "
                       "segments are connected in series)");
      } else {
        Serial.println("Falling back to SPI_CS not possible because it is 0");
        return false;
      }
    }
    return true;
  }
};

const static byte charTable[] PROGMEM = {
    B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111,
    B01110000, B01111111, B01111011, B01110111, B00011111, B00001101, B00111101,
    B01001111, B01000111, B00000000, B00000000, B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000, B00000000, B00000000, B10110000, B00100010,
    B00111111, B01011011, B10100101, B00110001, B00000010, B01001010, B01101000,
    B01000010, B00000111, B00000100, B00000001, B10000000, B00100101, B01111110,
    B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000,
    B01111111, B01111011, B01001000, B01011000, B01000011, B00001001, B01100001,
    B11100101, B01111101, B01110111, B00011111, B01001110, B00111101, B01001111,
    B01000111, B01011110, B00110111, B00000110, B00111100, B01010111, B00001110,
    B01010100, B01110110, B01111110, B01100111, B01101011, B01100110, B01011011,
    B00001111, B00111110, B00111110, B00101010, B00110111, B00111011, B01101101,
    B01001110, B00010011, B01111000, B01100010, B00001000, B00100000, B01111101,
    B00011111, B00001101, B00111101, B01101111, B01000111, B01111011, B00010111,
    B00000100, B00011000, B01010111, B00000110, B00010100, B00010101, B00011101,
    B01100111, B01110011, B00000101, B01011011, B00001111, B00011100, B00011100,
    B00010100, B00110111, B00111011, B01101101, B00110001, B00000110, B00000111,
    B01000000, B00000000};
