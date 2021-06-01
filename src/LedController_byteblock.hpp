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

namespace sakurajin {

    /**
     * \~english
     * @brief A block of bytes used to represent all of the data in a Segment.
     * It is simply a ByteRow with size 8 but useful regardless.
     *
     * \~german
     * @brief Ein Block an bytes, der alle Daten in einem Segment darstellt.
     * Es ist einfach eine ByteRow mit Länge 8, trotzdem nützlich.
     */
    class ByteBlock : public ByteRow<8> {
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
        ByteBlock(byte newdata[8]) noexcept:ByteRow<8>(newdata) {};

        /**
         * \~english
         * @brief Construct a new Byte Block object from a list of values.
         * This way {0,0,0,0,0,0,0,0} can be used as copy-assignement.
         *
         * \~german
         * @brief Erstelle ein neues Objekt aus einer Liste von Werten.
         * So kann {0,0,0,0,0,0,0,0} als Kopierzuweisung verwendet werden.
         */
        ByteBlock(byte n0,byte n1,byte n2,byte n3,byte n4,byte n5,byte n6,byte n7) noexcept:ByteBlock() {
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
        ByteBlock() noexcept:ByteRow<8>() {};

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
        ByteBlock(const ByteRow<8>& data) noexcept:ByteRow<8>(data) {};

        /**
         * \~english
         * @brief Turns an ByteBlock of rows into an ByteBlock of columns
         *
         * @return ByteBlock The Columns of the given ByteBlock
         *
         * \~german
         * @brief Wandelt einen Block aus Zeilen in einen Block aus Spalten.
         *
         * @return ByteBlock Die Splaten des gegebenen Blocks
         */
        ByteBlock transpose() const noexcept {
            return ByteBlock::makeColumns(*this);
        }

        /**
         * \~english
         * @brief Turns an ByteBlock of rows into an ByteBlock of columns
         *
         * @return ByteBlock The Columns of the given ByteBlock
         *
         * \~german
         * @brief Wandelt einen Block aus Zeilen in einen Block aus Spalten.
         *
         * @return ByteBlock Die Splaten des gegebenen Blocks
         */
        ByteBlock makeColumns() const noexcept {
            return this->transpose();
        }

        /**
         * \~english
         * @brief Reverse an ByteBlock of 8 bytes (mirror it)
         *
         * @return ByteBlock The reversed ByteBlock
         *
         * \~german
         * @brief Diese Funktion kehrt die Reihnfolge der einzelnen Zeilen um (spiegeln in x-Richtung).
         *
         * @return ByteBlock Der umgekehrte ByteBlock.
         */
        ByteBlock reverse() const noexcept {
            return ByteBlock::reverse(*this);
        }

        /**
         * \~english
         * @brief rotate an ByteBlock by 180 degrees
         *
         * @return ByteBlock The rotated ByteBlock
         *
         * \~german
         * @brief rotiert einen ByteBlock um 180 Grad
         *
         * @return ByteBlock Der rotierte byteBlock.
         */
        ByteBlock rotate180() const noexcept {
            return ByteBlock::rotate180(*this);
        }

        /**
         * \~english
         * @brief This function changes to bitorder of a byte (useful to mirror
         * "images" you want to display)
         *
         * @param input The byte that should be reversed
         * @return byte The reversed byte
         *
         * \~german
         * @brief Diese Funktion ändert die Bitreihnfolge eines Bytes bzw. kehrt ein Bit um.
         *
         * @param input Das byte, das umgekehrt werden soll.
         * @return byte Das umgekehrte Byte
         */
        static byte reverse(byte input) noexcept {
            byte ret = 0x00;
            for (unsigned int i = 0; i < 8; i++) {
                if (input & (0x01U << i)) {
                    ret |= 0x80U >> i;
                }
            }
            return ret;
        }

        /**
         * \~english
         * @brief Reverse an ByteBlock of 8 bytes (mirror it)
         *
         * @param input The ByteBlock that should be mirrored
         * @return ByteBlock The reversed ByteBlock
         *
         * \~german
         * @brief Diese Funktion kehrt die Reihnfolge der einzelnen Zeilen um (spiegeln in x-Richtung).
         *
         * @param input Der ByteBlock der umgekehrt werden soll.
         * @return ByteBlock Der umgekehrte ByteBlock.
         */
        static ByteBlock reverse(ByteBlock input) noexcept {
            auto reversedInput = ByteBlock();

            for (unsigned int i = 0; i < 8; i++) {
                reversedInput[i] = reverse(input[i]);
            }

            return reversedInput;
        }

        /**
         * \~english
         * @brief rotate an ByteBlock by 180 degrees
         *
         * @param input the ByteBlock that will be rotated
         * @return ByteBlock The rotated ByteBlock
         *
         * \~german
         * @brief rotiert einen ByteBlock um 180 Grad
         *
         * @param input Der zu rotierende ByteBlock.
         * @return ByteBlock Der rotierte byteBlock.
         */
        static ByteBlock rotate180(ByteBlock input) noexcept {
            auto rotatedInput = ByteBlock();

            for (unsigned int i = 0; i < 8; i++) {
                rotatedInput[7 - i] = reverse(input[i]);
            }

            return rotatedInput;
        }

        /**
         * \~english
         * @brief Turns an ByteBlock of rows into an ByteBlock of columns
         *
         * @param rowArray the ByteBlock of rows of which you want the columns of
         * @return ByteBlock The Columns of the given ByteBlock
         *
         * \~german
         * @brief Wandelt einen Block aus Zeilen in einen Block aus Spalten.
         *
         * @param rowArray Der Block aus Zeilen
         * @return ByteBlock Die Splaten des gegebenen Blocks
         */
        static ByteBlock makeColumns(ByteBlock rowArray) noexcept {
            auto columnArray = ByteBlock();

            for (unsigned int i = 0; i < 8; i++) {
                for (unsigned int j = 0; j < 8; j++) {
                    columnArray[7-i] |= (B10000000 & (rowArray[j]<<i)) >> (j);
                }
            }

            return reverse(columnArray);
        }

        /**
         * \~english
         * @brief Turns an ByteBlock of rows into an ByteBlock of columns
         *
         * @param rowArray the ByteBlock of rows of which you want the columns of
         * @return ByteBlock The Columns of the given ByteBlock
         *
         * \~german
         * @brief Wandelt einen Block aus Zeilen in einen Block aus Spalten.
         *
         * @param rowArray Der Block aus Zeilen
         * @return ByteBlock Die Splaten des gegebenen Blocks
         */
        static ByteBlock transpose(ByteBlock rowArray) noexcept {
            return ByteBlock::makeColumns(rowArray);
        }

    };

}
