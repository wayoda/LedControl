#pragma once

/**
 * @file sakurajin::LedController_display.hpp
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief This file contains the functions to display data on an sakurajin::LedController
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "LedController_template.hpp"
#include "LedController_MAX72XX.hpp"

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::resetBuffers() noexcept {
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
void sakurajin::LedController<columns,rows>::resetMatrix() noexcept {
    if (!initilized) {
        return;
    }

    activateAllSegments();
    setIntensity(1);
    clearMatrix();
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::clearMatrix() noexcept {
    if (!initilized) {
        return;
    }

    for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
        clearSegment(i); // Clear Segments
    }
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::displayOnSegment(unsigned int segmentindex,
        sakurajin::ByteBlock data) noexcept {
    if (!initilized || segmentindex >= conf.SegmentCount()) {
        return;
    }

    for (int i = 0; i < 8; i++) {
        setRow(segmentindex, i, data[i]);
    }
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::displayOnSegment(unsigned int column, unsigned int row_num, sakurajin::ByteBlock data) noexcept {
    displayOnSegment(conf.getSegmentNumber(column,row_num),data);
}


template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::shutdownSegment(unsigned int segmentNumber) noexcept {
    if (!initilized || segmentNumber >= conf.SegmentCount()) {
        return;
    }

    spiTransfer(segmentNumber, sakurajin::MAX72XX::OP_SHUTDOWN, 0);
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::activateSegment(unsigned int segmentNumber) noexcept {
    if (!initilized || segmentNumber >= conf.SegmentCount()) {
        return;
    }

    spiTransfer(segmentNumber, sakurajin::MAX72XX::OP_SHUTDOWN, 1);
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::shutdownAllSegments() noexcept {
    if (!initilized) {
        return;
    }

    for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
        shutdownSegment(i);
    }
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::activateAllSegments() noexcept {
    if (!initilized) {
        return;
    }

    for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
        activateSegment(i);
    }
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::clearSegment(unsigned int segmentNumber) noexcept {
    if (!initilized || segmentNumber >= conf.SegmentCount()) {
        return;
    }

    for (int i = 0; i < 8; i++) {
        LedStates[segmentNumber][i] = 0x00;
        spiTransfer(segmentNumber, i + 1, LedStates[segmentNumber][i]);
    }
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::setRow(unsigned int segmentNumber, unsigned int row,
        byte value) noexcept {
    if (!initilized || segmentNumber >= conf.SegmentCount() || row > 7 ||
            (conf.onlySendOnChange && LedStates[segmentNumber][row] == value)) {
        return;
    }

    LedStates[segmentNumber][row] = value;
    spiTransfer(segmentNumber, row + 1, LedStates[segmentNumber][row]);
}

template <size_t columns, size_t rows>
byte sakurajin::LedController<columns,rows>::getRow(unsigned int segmentNumber, unsigned int row) noexcept {
    if (!initilized || segmentNumber >= conf.SegmentCount() || row > 7) {
        return 0x00;
    }

    return LedStates[segmentNumber][row];
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::setLed(unsigned int segmentNumber, unsigned int row,
        unsigned int column, boolean state) noexcept {
    if (!initilized || row > 7 || column > 7 ||
            segmentNumber >= conf.SegmentCount()) {
        return;
    };

    byte val = 0x01 << column;

    if (state)
        val |= LedStates[segmentNumber][row];
    else {
        val = ~val;
        val &= LedStates[segmentNumber][row];
    }

    setRow(segmentNumber,row,val);
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::setColumn(unsigned int segmentNumber, unsigned int col,
        byte value) noexcept {
    if (!initilized || segmentNumber >= conf.SegmentCount() || col > 7) {
        return;
    };

    ByteBlock val = LedStates[segmentNumber].transpose();
    val[col] = value;
    displayOnSegment(segmentNumber,val.transpose());

}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::setDigit(unsigned int segmentNumber, unsigned int digit,
        byte value, boolean dp) noexcept {
    if (!initilized || segmentNumber >= conf.SegmentCount() || digit > 7) {
        return;
    };

    byte v = sakurajin::MAX72XX::getChar(value%16);
    if (dp) {
        v |= B10000000;
    };
    LedStates[segmentNumber][digit] = v;
    spiTransfer(segmentNumber, digit + 1, v);
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::setChar(unsigned int segmentNumber, unsigned int digit,
        char value, boolean dp) noexcept {
    if (!initilized || segmentNumber >= conf.SegmentCount() || digit > 7) {
        return;
    };

    byte v = sakurajin::MAX72XX::getChar(value);
    if (dp) {
        v |= B10000000;
    };

    LedStates[segmentNumber][digit] = v;
    spiTransfer(segmentNumber, digit + 1, v);
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::refreshSegments() noexcept {
    if (!initilized) {
        return;
    }

    for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
        refreshSegment(i);
    }
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::updateSegments() noexcept {
    if (!initilized) {
        return;
    }

    for (unsigned int seg = 0; seg < conf.SegmentCount(); seg++) {
        updateSegment(seg);
    }
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::updateSegment(unsigned int segmentNumber) noexcept {
    if (!initilized) {
        return;
    }

    for (unsigned int row = 0; row < 8; row++) {
        spiTransfer(segmentNumber, row + 1, LedStates[segmentNumber][row]);
    }
}

