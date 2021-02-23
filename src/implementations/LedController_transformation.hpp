#pragma once

/**
 * @file LedController_transformation.hpp
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief This file contains the functions that transform given data (eg. rotation,reverse,...)
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "LedController_template.hpp"

template <size_t columns, size_t rows>
byte LedController<columns,rows>::reverse(byte var) {
    return ByteBlock::reverse(var);
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::makeColumns(ByteBlock rowArray) {
    return rowArray.makeColumns();
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::reverse(ByteBlock input) {
    return input.reverse();
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::rotate180(ByteBlock input) {
    return input.rotate180();
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::makeColumns(ByteBlock rowArray, ByteBlock *columnArray) {
    if (columnArray == nullptr) {
        return;
    };

    *columnArray = rowArray.makeColumns();
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::reverse(ByteBlock input, ByteBlock *reversedInput) {
    if (reversedInput == nullptr) {
        return;
    }

    *reversedInput = input.reverse();
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::rotate180(ByteBlock input, ByteBlock *rotatedInput) {
    if (rotatedInput == nullptr) {
        return;
    }

    *rotatedInput = input.rotate180();
}