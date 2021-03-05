#pragma once
/**
 * @file sakurajin::LedController_low_level.hpp
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief This file contains the low level functions of the sakurajin::LedController which are needed to implement most external functions.
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "LedController_template.hpp"
#include "LedController_MAX72XX.hpp"

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::spiTransfer(unsigned int segment, byte opcode, byte data) noexcept {
    if (!initilized || segment >= conf.SegmentCount()) {
        return;
    }

    // Create an array with the data to shift out
    unsigned int offset = conf.getColumn(segment) * 2;
    unsigned int row = conf.getRow(segment);
    unsigned int maxbytes = conf.SegmentCount() * 2;

    for(unsigned int i = 0; i < maxbytes; i++) {
        spidata[i] = i%2 == 0 ? 0x00 : sakurajin::MAX72XX::OP_NOOP;
    }

    // put our device data into the array
    spidata[row*columns*2+offset+1] = opcode;
    spidata[row*columns*2+offset] = data;

    if(conf.virtual_multi_row && conf.SPI_CS != 0) {
        auto cs = conf.SPI_CS;
        digitalWrite(cs, LOW);

        if (conf.useHardwareSpi) {
            SPI.beginTransaction(SPISettings(conf.spiTransferSpeed, MSBFIRST, SPI_MODE0));
        }

        for (int i = maxbytes; i > 0; i--) {
            if (conf.useHardwareSpi) {
                SPI.transfer(spidata[i-1]);
            } else {
                shiftOut(conf.SPI_MOSI, conf.SPI_CLK, MSBFIRST, spidata[i-1]);
            }
        }

        //end the spi transfer if hardware should be used
        if (conf.useHardwareSpi) {
            SPI.endTransaction();
        }

        // latch the data onto the display
        digitalWrite(cs, HIGH);

    } else {
        for(unsigned int r = 0; r < rows ; r++) {

            //enable the line
            auto cs = conf.row_SPI_CS[r];
            digitalWrite(cs, LOW);

            //init the spi transfer if hardware should be used
            if (conf.useHardwareSpi) {
                SPI.beginTransaction(SPISettings(conf.spiTransferSpeed, MSBFIRST, SPI_MODE0));
            }

            // Now shift out the data
            for (int i = columns*2; i > 0; i--) {
                if (conf.useHardwareSpi) {
                    SPI.transfer(spidata[r*columns*2+i-1]);
                } else {
                    shiftOut(conf.SPI_MOSI, conf.SPI_CLK, MSBFIRST, spidata[r*columns*2+i-1]);
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
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::setScanLimit(unsigned int segmentNumber,
        unsigned int limit) noexcept {
    if (!initilized || segmentNumber >= conf.SegmentCount()) {
        return;
    };
    if (limit < 8) {
        spiTransfer(segmentNumber, sakurajin::MAX72XX::OP_SCANLIMIT, limit);
    };
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::setIntensity(unsigned int newIntesityLevel) noexcept {
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
void sakurajin::LedController<columns,rows>::setIntensity(unsigned int segmentNumber,
        unsigned int newIntesityLevel) noexcept {
    if (newIntesityLevel > 15 || !initilized ||
            segmentNumber >= conf.SegmentCount()) {
        return;
    }

    spiTransfer(segmentNumber, sakurajin::MAX72XX::OP_INTENSITY, newIntesityLevel);
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::refreshSegment(unsigned int segmentNumber) noexcept {
    if (!initilized) {
        return;
    }

    spiTransfer(segmentNumber, sakurajin::MAX72XX::OP_DISPLAYTEST, 0);
    // scanlimit is set to max on startup
    setScanLimit(segmentNumber, 7);
    // decode is done in source
    spiTransfer(segmentNumber, sakurajin::MAX72XX::OP_DECODEMODE, 0);
    clearSegment(segmentNumber);
    // we go into shutdown-mode on startup
    activateSegment(segmentNumber);

    setIntensity(segmentNumber, this->conf.IntensityLevel);

    updateSegment(segmentNumber);
}
