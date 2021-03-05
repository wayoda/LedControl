#pragma once

/**
 * @file sakurajin::LedController_transformation.hpp
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
byte sakurajin::LedController<columns,rows>::reverse(byte var) noexcept {
    return sakurajin::ByteBlock::reverse(var);
}

template <size_t columns, size_t rows>
sakurajin::ByteBlock sakurajin::LedController<columns,rows>::makeColumns(sakurajin::ByteBlock rowArray) noexcept {
    return rowArray.makeColumns();
}

template <size_t columns, size_t rows>
sakurajin::ByteBlock sakurajin::LedController<columns,rows>::reverse(sakurajin::ByteBlock input) noexcept {
    return input.reverse();
}

template <size_t columns, size_t rows>
sakurajin::ByteBlock sakurajin::LedController<columns,rows>::rotate180(sakurajin::ByteBlock input) noexcept {
    return input.rotate180();
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::makeColumns(sakurajin::ByteBlock rowArray, sakurajin::ByteBlock *columnArray) noexcept {
    if (columnArray == nullptr) {
        return;
    };

    *columnArray = rowArray.makeColumns();
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::reverse(sakurajin::ByteBlock input, sakurajin::ByteBlock *reversedInput) noexcept {
    if (reversedInput == nullptr) {
        return;
    }

    *reversedInput = input.reverse();
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::rotate180(sakurajin::ByteBlock input, sakurajin::ByteBlock *rotatedInput) noexcept {
    if (rotatedInput == nullptr) {
        return;
    }

    *rotatedInput = input.rotate180();
}