#pragma once

/**
 * @file LedController_display.hpp
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief This file contains the functions to display data on an LedController
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "LedController_template.hpp"

template <size_t columns, size_t rows>
void LedController<columns,rows>::resetBuffers() {
    for (unsigned int j = 0; j < rows*columns*2; j++) {
        spidata[j] = j%2 == 0 ? 0x00 : sakurajin::MAX72XX::OP_NOOP;  
    }

    for (unsigned int j = 0; j < conf.SegmentCount(); j++) {
        for (unsigned int i = 0; i < 8; i++) {
            LedStates[j][i] = 0x00;
        }
    }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::resetMatrix() {
    if (!initilized) {
        return;
    }

    activateAllSegments();
    setIntensity(1);
    clearMatrix();
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::clearMatrix() {
    if (!initilized) {
        return;
    }

    for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
        clearSegment(i); // Clear Segments
    }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::displayOnSegment(unsigned int segmentindex,
        sakurajin::ByteBlock data) {
    if (!initilized || segmentindex >= conf.SegmentCount()) {
        return;
    }

    for (int i = 0; i < 8; i++) {
        setRow(segmentindex, i, data[i]);
    }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::displayOnSegment(unsigned int column, unsigned int row_num, sakurajin::ByteBlock data) {
    displayOnSegment(conf.getSegmentNumber(column,row_num),data);
}


template <size_t columns, size_t rows>
void LedController<columns,rows>::shutdownSegment(unsigned int segmentNumber) {
    if (!initilized || segmentNumber >= conf.SegmentCount()) {
        return;
    }

    spiTransfer(segmentNumber, sakurajin::MAX72XX::OP_SHUTDOWN, 0);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::activateSegment(unsigned int segmentNumber) {
    if (!initilized || segmentNumber >= conf.SegmentCount()) {
        return;
    }

    spiTransfer(segmentNumber, sakurajin::MAX72XX::OP_SHUTDOWN, 1);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::shutdownAllSegments() {
    if (!initilized) {
        return;
    }

    for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
        shutdownSegment(i);
    }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::activateAllSegments() {
    if (!initilized) {
        return;
    }

    for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
        activateSegment(i);
    }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::clearSegment(unsigned int segmentNumber) {
    if (!initilized || segmentNumber >= conf.SegmentCount()) {
        return;
    }

    for (int i = 0; i < 8; i++) {
        LedStates[segmentNumber][i] = 0x00;
        spiTransfer(segmentNumber, i + 1, LedStates[segmentNumber][i]);
    }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setRow(unsigned int segmentNumber, unsigned int row,
        byte value) {
    if (!initilized || segmentNumber >= conf.SegmentCount() || row > 7 ||
            (conf.onlySendOnChange && LedStates[segmentNumber][row] == value)) {
        return;
    }

    LedStates[segmentNumber][row] = value;
    spiTransfer(segmentNumber, row + 1, LedStates[segmentNumber][row]);
}

template <size_t columns, size_t rows>
byte LedController<columns,rows>::getRow(unsigned int segmentNumber, unsigned int row) {
    if (!initilized || segmentNumber >= conf.SegmentCount() || row > 7) {
        return 0x00;
    }

    return LedStates[segmentNumber][row];
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setLed(unsigned int segmentNumber, unsigned int row,
        unsigned int column, boolean state) {
    if (!initilized || row > 7 || column > 7 ||
            segmentNumber >= conf.SegmentCount()) {
        return;
    };

    byte val = 0x80 >> column;

    if (state)
        val |= LedStates[segmentNumber][row];
    else {
        val = ~val;
        val &= LedStates[segmentNumber][row];
    }

    setRow(segmentNumber,row,val);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setColumn(unsigned int segmentNumber, unsigned int col,
        byte value) {
    if (!initilized || segmentNumber >= conf.SegmentCount() || col > 7) {
        return;
    };

    byte val;

    for (int row = 0; row < 8; row++) {
        val = value >> (7 - row);
        val &= 0x01;
        val <<= col;
        val |= LedStates[segmentNumber][row];
        setRow(segmentNumber, row, val);
    }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setDigit(unsigned int segmentNumber, unsigned int digit,
        byte value, boolean dp) {
    if (!initilized || segmentNumber >= conf.SegmentCount() || digit > 7 ||
            value > 15) {
        return;
    };

    byte v = pgm_read_byte_near(sakurajin::MAX72XX::charTable + value);
    if (dp) {
        v |= B10000000;
    };
    LedStates[segmentNumber][digit] = v;
    spiTransfer(segmentNumber, digit + 1, v);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setChar(unsigned int segmentNumber, unsigned int digit,
        char value, boolean dp) {
    if (!initilized || segmentNumber >= conf.SegmentCount() || digit > 7) {
        return;
    };

    byte index = (byte)value;
    if (index > 127) {
        // no defined beyond index 127, so we use the space char
        index = 32;
    }

    byte v = pgm_read_byte_near(sakurajin::MAX72XX::charTable + index);
    if (dp) {
        v |= B10000000;
    };

    LedStates[segmentNumber][digit] = v;
    spiTransfer(segmentNumber, digit + 1, v);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::refreshSegments() {
    if (!initilized) {
        return;
    }

    for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
        refreshSegment(i);
    }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::updateSegments() {
    if (!initilized) {
        return;
    }

    for (unsigned int seg = 0; seg < conf.SegmentCount(); seg++) {
        updateSegment(seg);
    }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::updateSegment(unsigned int segmentNumber) {
    if (!initilized) {
        return;
    }

    for (unsigned int row = 0; row < 8; row++) {
        spiTransfer(segmentNumber, row + 1, LedStates[segmentNumber][row]);
    }
}

