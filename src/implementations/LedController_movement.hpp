#pragma once
/**
 * @file sakurajin::LedController_movement.hpp
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief This file contains the functions to move data on the sakurajin::LedController
 * @version 0.1
 * @date 2020-12-21
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "LedController_template.hpp"

template <size_t columns, size_t rows>
byte sakurajin::LedController<columns,rows>::moveRowRight(byte shiftedInColumn, unsigned int row_num) noexcept {
    if (!initilized || row_num >= rows) {
        return 0x00;
    }

    byte returnValue = 0x00;

    for (unsigned int i = 0; i < 8; i++) {
        if (LedStates[conf.getSegmentNumber(columns-1,row_num)][i] & 0x80) {
            returnValue |= 0x01 << i;
        };
    }

    for (int col = columns-1; col >= 0; col--) {
        auto seg = conf.getSegmentNumber(col,row_num);
        auto seg1 = conf.getSegmentNumber(col-1,row_num);

        for(unsigned int i = 0; i < 8; i++) {
            LedStates[seg][i] = LedStates[seg][i] << 1;
            if (seg != conf.getSegmentNumber(0,row_num) && LedStates[seg1][i] & 0x80) {
                LedStates[seg][i] |= 0x01;
            };
        }
    }

    setColumn(conf.getSegmentNumber(conf.getSegmentNumber(0,row_num),row_num), 0, shiftedInColumn);

    updateSegments();

    return returnValue;
}

template <size_t columns, size_t rows>
byte sakurajin::LedController<columns,rows>::moveRowLeft(byte shiftedInColumn, unsigned int row_num) noexcept {
    if (!initilized || row_num >= rows) {
        return 0x00;
    }

    byte returnValue = 0x00;

    for (unsigned int i = 0; i < 8; i++) {
        if (LedStates[conf.getSegmentNumber(0,row_num)][i] & 0x01) {
            returnValue |= 0x01 << i;
        };
    }


    for (unsigned int col = 0; col < columns; col++) {
        auto seg = conf.getSegmentNumber(col,row_num);
        auto seg1 = conf.getSegmentNumber(col+1,row_num);
        for(unsigned int i = 0; i < 8; i++) {
            LedStates[seg][i] = LedStates[seg][i] >> 1;
            if (seg != conf.getSegmentNumber(columns-1,row_num) && LedStates[seg1][i] & 0x01) {
                LedStates[seg][i] |= 0x80;
            };
        }
    }

    setColumn(conf.getSegmentNumber(columns-1,row_num), 7, shiftedInColumn);

    updateSegments();

    return returnValue;
}

template <size_t columns, size_t rows>
byte sakurajin::LedController<columns,rows>::moveColumnUp(byte shiftedInRow, unsigned int col_num) noexcept {
    if (!initilized) {
        return 0x00;
    }
    col_num%=columns;

    byte retval = LedStates[conf.getSegmentNumber(col_num,rows-1)][7];

    for (unsigned int i = rows-1; i>=0; i--) {
        auto seg = conf.getSegmentNumber(col_num,i);
        for(unsigned int j = 7; j>=0; j--) {
            if(j==0) {
                if(i == 0) {
                    LedStates[seg][0] = shiftedInRow;
                } else {
                    LedStates[seg][0] = LedStates[conf.getSegmentNumber(col_num,i-1)][7];
                }
            } else {
                LedStates[seg][j] = LedStates[seg][j - 1];
            }
            if(j == 0) {
                break;
            };
        }
        if(i == 0) {
            break;
        };
    }

    updateSegments();

    return retval;
}

template <size_t columns, size_t rows>
byte sakurajin::LedController<columns,rows>::moveColumnDown(byte shiftedInRow, unsigned int col_num) noexcept {
    if (!initilized) {
        return 0x00;
    }

    byte retval = LedStates[conf.getSegmentNumber(col_num,0)][0];

    for (unsigned int i = 0; i<rows; i++) {
        auto seg = conf.getSegmentNumber(col_num,i);
        for(unsigned int j = 0; j<8; j++) {
            if(j==7) {
                if(i == rows-1) {
                    LedStates[seg][7] = shiftedInRow;
                } else {
                    LedStates[seg][7] = LedStates[conf.getSegmentNumber(col_num,i+1)][0];
                }
            } else {
                LedStates[seg][j] = LedStates[seg][j + 1];
            }
        }

    }

    updateSegments();

    return retval;
}

//functions that abstract the basic functions above
template <size_t columns, size_t rows>
sakurajin::ByteRow<columns> sakurajin::LedController<columns,rows>::moveDown() noexcept {
    return moveDown(sakurajin::ByteRow<columns>());
}

template <size_t columns, size_t rows>
sakurajin::ByteRow<columns> sakurajin::LedController<columns,rows>::moveUp() noexcept {
    return moveUp(sakurajin::ByteRow<columns>());
}

template <size_t columns, size_t rows>
sakurajin::ByteRow<rows> sakurajin::LedController<columns,rows>::moveLeft() noexcept {
    return moveLeft(sakurajin::ByteRow<rows>());
}

template <size_t columns, size_t rows>
sakurajin::ByteRow<rows> sakurajin::LedController<columns,rows>::moveRight() noexcept {
    return moveRight(sakurajin::ByteRow<rows>());
}

template <size_t columns, size_t rows>
sakurajin::ByteRow<rows> sakurajin::LedController<columns,rows>::moveLeft(const sakurajin::ByteRow<rows>& shiftedInColumn) noexcept {
    sakurajin::ByteRow<rows> ret = sakurajin::ByteRow<rows>();
    for(unsigned int i = 0; i < rows; i++) {
        ret[i] = moveRowLeft(shiftedInColumn[i],i);
    }
    return ret;
}

template <size_t columns, size_t rows>
sakurajin::ByteRow<rows> sakurajin::LedController<columns,rows>::moveRight(const sakurajin::ByteRow<rows>& shiftedInColumn) noexcept {
    sakurajin::ByteRow<rows> ret = sakurajin::ByteRow<rows>();
    for(unsigned int i = 0; i < rows; i++) {
        ret[i] = moveRowRight(shiftedInColumn[i],i);
    }
    return ret;
}

template <size_t columns, size_t rows>
sakurajin::ByteRow<columns> sakurajin::LedController<columns,rows>::moveUp(const sakurajin::ByteRow<columns>& shiftedInRow) noexcept {
    sakurajin::ByteRow<columns> ret = sakurajin::ByteRow<columns>();
    for(unsigned int i = 0; i < columns; i++) {
        ret[i] = moveColumnUp(shiftedInRow[i],i);
    }
    return ret;
}

template <size_t columns, size_t rows>
sakurajin::ByteRow<columns> sakurajin::LedController<columns,rows>::moveDown(const sakurajin::ByteRow<columns>& shiftedInRow) noexcept {
    sakurajin::ByteRow<columns> ret = sakurajin::ByteRow<columns>();
    for(unsigned int i = 0; i < columns; i++) {
        ret[i] = moveColumnDown(shiftedInRow[i],i);
    }
    return ret;
}

//deprecated functions that will be removed in the future

//removed in version 2.1.0
template <size_t columns, size_t rows>
byte sakurajin::LedController<columns,rows>::moveLeft(byte shiftedInColumn) noexcept {
    return moveRowLeft(shiftedInColumn,0);
}

//removed in version 2.1.0
template <size_t columns, size_t rows>
byte sakurajin::LedController<columns,rows>::moveRight(byte shiftedInColumn) noexcept {
    return moveRowRight(shiftedInColumn,0);
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::moveUp(sakurajin::ByteRow<columns>* shiftedOutRow) noexcept {
    moveUp(sakurajin::ByteRow<columns>(), shiftedOutRow);
};

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::moveDown(sakurajin::ByteRow<columns>* shiftedOutRow) noexcept {
    moveDown(sakurajin::ByteRow<columns>(), shiftedOutRow);
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::moveDown(const sakurajin::ByteRow<columns>& shiftedInRow, sakurajin::ByteRow<columns>* shiftedOutRow) noexcept {
    if(shiftedOutRow == nullptr) {
        moveDown(shiftedInRow);
    } else {
        *shiftedOutRow = moveDown(shiftedInRow);
    }
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::moveUp(const sakurajin::ByteRow<columns>& shiftedInRow, sakurajin::ByteRow<columns>* shiftedOutRow) noexcept {
    if(shiftedOutRow == nullptr) {
        moveUp(shiftedInRow);
    } else {
        *shiftedOutRow = moveUp(shiftedInRow);
    }
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::moveLeft(const sakurajin::ByteRow<rows>& shiftedInColumn, sakurajin::ByteRow<rows>* shiftedOutColumn) noexcept {
    if(shiftedOutColumn == nullptr) {
        moveLeft(shiftedInColumn);
    } else {
        *shiftedOutColumn = moveLeft(shiftedInColumn);
    }
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::moveRight(const sakurajin::ByteRow<rows>& shiftedInColumn, sakurajin::ByteRow<rows>* shiftedOutColumn) noexcept {
    if(shiftedOutColumn == nullptr) {
        moveRight(shiftedInColumn);
    } else {
        *shiftedOutColumn = moveRight(shiftedInColumn);
    }
}
