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
  byte ret = 0x00;
  for (unsigned int i = 0; i < 8; i++) {
    if (var & (0x01U << i)) {
      ret |= 0x80U >> i;
    }
  }
  return ret;
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::makeColumns(ByteBlock rowArray) {
  auto columnArray = ByteBlock();

  for (unsigned int i = 0; i < 8; i++) {
    columnArray[i] = 0x00;
    for (unsigned int j = 0; j < 8; j++) {
      columnArray[i] |= (0x80 & (rowArray[j]<<i)) >> (7 - j);
    }
  }

  return rotate180(columnArray);
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::reverse(ByteBlock input) {
  auto reversedInput = ByteBlock();

  for (unsigned int i = 0; i < 8; i++) {
    reversedInput[i] = reverse(input[i]);
  }

  return reversedInput;
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::rotate180(ByteBlock input) {
  auto rotatedInput = ByteBlock();

  for (unsigned int i = 0; i < 8; i++) {
    rotatedInput[7 - i] = reverse(input[i]);
  }

  return rotatedInput;
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::makeColumns(ByteBlock rowArray, ByteBlock *columnArray) {
  if (!initilized || columnArray == nullptr) {
    return;
  };

  for (unsigned int i = 0; i < 8; i++) {
    (*columnArray)[i] = 0x00;
    for (unsigned int j = 0; j < 8; j++) {
      (*columnArray)[i] |= (0x01 & (rowArray[j] >> (7 - i))) << (7 - j);
    }
  }
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::reverse(ByteBlock input, ByteBlock *reversedInput) {
  if (reversedInput == nullptr) {
    return;
  }

  for (unsigned int i = 0; i < 8; i++) {
    (*reversedInput)[i] = reverse(input[i]);
  }
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::rotate180(ByteBlock input, ByteBlock *rotatedInput) {
  if (rotatedInput == nullptr) {
    return;
  }

  for (unsigned int i = 0; i < 8; i++) {
    (*rotatedInput)[7 - i] = reverse(input[i]);
  }
}