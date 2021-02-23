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

#include "LedController_byterow.hpp"

/**
 * \~english
 * @brief A block of bytes used to represent all of the data in a Segment.
 * It is simply a ByteRow with size 8 but useful regardless.
 * 
 * \~german
 * @brief Ein Block an bytes, der alle Daten in einem Segment darstellt.
 * Es ist einfach eine ByteRow mit Länge 8, trotzdem nützlich.
 */
class ByteBlock : public ByteRow<8>{
public:

    /**
     * \~english
     * @brief Construct a new Byte Block object from an array.
     * 
     * @param newdata The array which should be used to initilize the row.
     * 
     * \~german
     * @brief Konstruiere ein neues Object aus einem Array.
     * 
     * @param newdata Das Array, das zum initialisieren verwendet werden soll.
     */
    ByteBlock(byte newdata[8]):ByteRow<8>(newdata){};

    /**
     * \~english
     * @brief Construct a new Byte Block object from a list of values.
     * This way {0,0,0,0,0,0,0,0} can be used as copy-assignement.
     * 
     * \~german
     * @brief Erstelle ein neues Objekt aus einer Liste von Werten.
     * So kann {0,0,0,0,0,0,0,0} als Kopierzuweisung verwendet werden.
     */
    ByteBlock(byte n0,byte n1,byte n2,byte n3,byte n4,byte n5,byte n6,byte n7):ByteBlock(){
        _data[0] = n0;
        _data[1] = n1;
        _data[2] = n2;
        _data[3] = n3;
        _data[4] = n4;
        _data[5] = n5;
        _data[6] = n6;
        _data[7] = n7;
    };

    /**
     * \~english
     * @brief Construct a new Byte Block object with 0s.
     * 
     * \~german
     * @brief Konstruiere ein neuess Objekt mit 0en.
     */
    ByteBlock():ByteRow<8>(){};

    /**
     * \~english
     * @brief Construct a new Byte Block object from a byteRow.
     * @note This constructor mainly exists for automatic casting.
     * @param data the data that should be assigned
     * 
     * \~german
     * @brief Konstruiere ein neuess Objekt aus einer ByteRow.
     * @note Dieser Konstruktor existiert hauptsächlich für Typumwandlungen.
     * @param data Die daten, die zugewiesen werden sollen
     */
    ByteBlock(const ByteRow<8>& data):ByteRow<8>(data){};

    ByteBlock transpose() const{
        return ByteBlock::makeColumns(*this);
    }

    ByteBlock makeColumns() const{
        return this->transpose();
    }

    ByteBlock reverse() const{
        return ByteBlock::reverse(*this);
    }

    ByteBlock rotate180() const{
        return ByteBlock::rotate180(*this);
    }

    static byte reverse(byte var) {
        byte ret = 0x00;
        for (unsigned int i = 0; i < 8; i++) {
            if (var & (0x01U << i)) {
                ret |= 0x80U >> i;
            }
        }
        return ret;
    }

    static ByteBlock reverse(ByteBlock input) {
        auto reversedInput = ByteBlock();

        for (unsigned int i = 0; i < 8; i++) {
            reversedInput[i] = reverse(input[i]);
        }

        return reversedInput;
    }

    static ByteBlock rotate180(ByteBlock input) {
        auto rotatedInput = ByteBlock();

        for (unsigned int i = 0; i < 8; i++) {
            rotatedInput[7 - i] = reverse(input[i]);
        }

        return rotatedInput;
    }

    static ByteBlock makeColumns(ByteBlock rowArray) {
        auto columnArray = ByteBlock();

        for (unsigned int i = 0; i < 8; i++) {
            for (unsigned int j = 0; j < 8; j++) {
                columnArray[i] |= (B10000000 & (rowArray[j]<<i)) >> (j);
            }
        }

        return rotate180(columnArray);
    }
     
};
