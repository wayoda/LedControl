#pragma once
/**
 * @file LedController_low_level.hpp
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief This file contains the low level functions of the LedController which are needed to implement most external functions.
 * @version 0.1
 * @date 2020-12-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "LedController_template.hpp"

// the opcodes for the MAX7221 and MAX7219
#define OP_NOOP 0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE 9
#define OP_INTENSITY 10
#define OP_SCANLIMIT 11
#define OP_SHUTDOWN 12
#define OP_DISPLAYTEST 15

template <size_t columns, size_t rows>
void LedController<columns,rows>::spiTransfer(unsigned int segment, byte opcode, byte data) {
  if (!initilized || segment >= conf.SegmentCount()) {
    return;
  }

  // Create an array with the data to shift out
  unsigned int offset = conf.getColumn(segment) * 2;
  unsigned int row = conf.getRow(segment);
  unsigned int maxbytes = conf.SegmentCount() * 2;

  for (unsigned int j = 0; j < rows;j++){
    for (unsigned int i = 0; i < columns*2; i++) {
      spidata[j][i] = 0x00;
    }
  }

  // put our device data into the array
  spidata[row][offset + 1] = opcode;
  spidata[row][offset] = data;
  
  for(unsigned int r = 0; r < rows ;r++){

    //enable the line
    auto cs = (conf.virtual_multi_row && conf.SPI_CS != 0) ? conf.SPI_CS : conf.row_SPI_CS[r];
    digitalWrite(cs, LOW);

    //init the spi transfer if hardware should be used
    if (conf.useHardwareSpi) {
      SPI.beginTransaction(SPISettings(conf.spiTransferSpeed, MSBFIRST, SPI_MODE0));
    }

    // Now shift out the data
    for (int i = maxbytes; i > 0; i--) {
      if (conf.useHardwareSpi) {
        SPI.transfer(spidata[r][i - 1]);
      } else {
        shiftOut(conf.SPI_MOSI, conf.SPI_CLK, MSBFIRST, spidata[r][i - 1]);
      }
    }

    //end the spi transfer if hardware should be used
    if (conf.useHardwareSpi) {
      SPI.endTransaction();
    }

    // latch the data onto the display
    digitalWrite(cs, HIGH);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setScanLimit(unsigned int segmentNumber,
                                 unsigned int limit) {
  if (!initilized || segmentNumber >= conf.SegmentCount()) {
    return;
  };
  if (limit < 8) {
    spiTransfer(segmentNumber, OP_SCANLIMIT, limit);
  };
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setIntensity(unsigned int newIntesityLevel) {
  if (newIntesityLevel > 15 || !initilized) {
    return;
  }

  // std::lock_guard<std::mutex> lock(mut_IntensityLevel);
  conf.IntensityLevel = newIntesityLevel;

  for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
    setIntensity(i, conf.IntensityLevel);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setIntensity(unsigned int segmentNumber,
                                 unsigned int newIntesityLevel) {
  if (newIntesityLevel > 15 || !initilized ||
      segmentNumber >= conf.SegmentCount()) {
    return;
  }

  spiTransfer(segmentNumber, OP_INTENSITY, newIntesityLevel);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::refreshSegment(unsigned int segmentNumber) {
  if (!initilized) {
    return;
  }

  spiTransfer(segmentNumber, OP_DISPLAYTEST, 0);
  // scanlimit is set to max on startup
  setScanLimit(segmentNumber, 7);
  // decode is done in source
  spiTransfer(segmentNumber, OP_DECODEMODE, 0);
  clearSegment(segmentNumber);
  // we go into shutdown-mode on startup
  activateSegment(segmentNumber);

  setIntensity(segmentNumber, this->conf.IntensityLevel);

  updateSegment(segmentNumber);
}
