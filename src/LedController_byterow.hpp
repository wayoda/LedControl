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

namespace sakurajin {

    /**
     * \~english
     * @brief This class is used to handle a row of bytes with a given length.
     *
     * @tparam SIZE The length of the row.
     *
     * \~german
     * @brief Diese Klasse wird genutzt, um eine Reihe aus bytes mit bekannter Länge zu handhaben.
     *
     * @tparam SIZE Die Länge der Reihe.
     */
    template<size_t SIZE>
    class ByteRow {
      protected:
        /**
         * \~english
         * @brief The internal array where the actual data is.
         *
         * \~german
         * @brief Das interne Array, was die Daten speichert.
         */
        byte _data[SIZE];

      public:

        /**
         * \~english
         * @brief Construct a new Byte Row object from an array.
         *
         * @param newdata The array which should be used to initilize the row.
         *
         * \~german
         * @brief Konstruiere ein neues Object aus einem Array.
         *
         * @param newdata Das Array, das zum initialisieren verwendet werden soll.
         */
        ByteRow(byte newdata[SIZE]) noexcept:ByteRow() {
            for(uint64_t i = 0; i < SIZE; i++) {
                _data[i] = newdata[i];
            }
        };

        /**
         * \~english
         * @brief Construct a new Byte Row object with 0s.
         *
         * \~german
         * @brief Konstruiere ein neuess Objekt mit 0en.
         */
        ByteRow() noexcept {
            for(uint64_t i = 0; i < SIZE; i++) {
                _data[i] = 0;
            }
        };

        /**
         * \~english
         * @brief Destroy the Byte Row object.
         *
         * \~german
         * @brief Lösche das Objekt.
         */
        ~ByteRow() noexcept {};

        /**
         * \~english
         * @brief Access the class just like the underlying array.
         *
         * @param index the index that is requested.
         * @return byte& the data at the requested index.
         *
         * \~german
         * @brief Greife auf das Objekt zu, wie auf das unterliegende Array.
         *
         * @param index Der Angefragte Index.
         * @return byte& Die Daten am angefragtem Index.
         */
        byte& operator[] (uint64_t index) noexcept {
            index %= SIZE;
            return _data[index];
        }

        /**
         * \~english
         * @brief Access the class just like the underlying array.
         *
         * @param index the index that is requested.
         * @return const byte& the data at the requested index.
         *
         * \~german
         * @brief Greife auf das Objekt zu, wie auf das unterliegende Array.
         *
         * @param index Der Angefragte Index.
         * @return byte& Die Daten am angefragtem Index.
         */
        const byte& operator[] (uint64_t index) const noexcept {
            index %= SIZE;
            return _data[index];
        }

        /**
         * \~english
         * @brief Access the class just like the underlying array.
         *
         * @param index he index that is requested.
         * @return byte the data at the requested index.
         *
         * \~german
         * @brief Greife auf das Objekt zu, wie auf das unterliegende Array.
         *
         * @param index Der Angefragte Index.
         * @return byte& Die Daten am angefragtem Index.
         */
        byte at(uint64_t index) const noexcept {
            index %= SIZE;
            return _data[index];
        }

        /**
         * \~english
         * @brief Assign an array to this object.
         *
         * @param newdata The data that will be assigned to this obejct
         * @return ByteRow& The modified object
         *
         * \~german
         * @brief Weise diesem Objekt ein array zu.
         *
         * @param newdata Die Array, das zugewiesen wird.
         * @return ByteRow& Das modifizierte Objekt.
         */
        ByteRow& operator= (byte newdata[SIZE]) noexcept {
            for(uint64_t i = 0; i < SIZE; i++) {
                _data[i] = newdata[i];
            }
            return *this;
        }

        /**
         * \~english
         * @brief Overlaps two ByteRows (bitwise or).
         * This operator can be used to display two ByteRows in one ByteRow.
         * @param other The ByteRow that should be overlapped with this object
         * @return ByteRow The resulting ByteRow
         *
         * \~german
         * @brief Überlappt zwei ByteRow Objeckte (bitweises oder)
         * Dieser operator kann genutzt werden, um zwei ByteRow Objekte in einem Anzuzeigen.
         * @param other Die ByteRows, welche mit diesem Objekt überladen werden soll
         * @return ByteRow Die resultierende ByteRow
         */
        virtual ByteRow operator&(const ByteRow<SIZE>& other) const noexcept {
            ByteRow<SIZE> block = ByteRow<SIZE>();
            for(unsigned int i = 0; i < SIZE; i++) {
                block[i] = (this->at(i)) | (other.at(i));
            }
            return block;
        }

        /**
         * \~english
         * @brief Moves all data of the ByteRow to the left by a given distance.
         * @note each index is handled on its own and no data is shifted across
         * @param distance the distance the data should be moved
         * @return ByteRow the resulting ByteRow
         *
         * \~german
         * @brief Schiebt die Daten um eine gegebene Distanz nach links.
         * @note Jeder Wert wird einzeln geschoben und nicht alles als eine Reihe.
         * @param distance Die Distanz die geschoben werden soll
         * @return ByteRow Die resultierende ByteRow
         */
        virtual ByteRow operator<<(unsigned int distance) const noexcept {
            ByteRow<SIZE> block = ByteRow<SIZE>();
            for(unsigned int i = 0; i < SIZE; i++) {
                block[i] = (this->at(i))>>distance;
            }
            return block;
        }

        /**
         * \~english
         * @brief Moves all data of the ByteRow to the right by a given distance.
         * @note each index is handled on its own and no data is shifted across
         * @param distance the distance the data should be moved
         * @return ByteRow the resulting ByteRow
         *
         * \~german
         * @brief Schiebt die Daten um eine gegebene Distanz nach rechts.
         * @note Jeder Wert wird einzeln geschoben und nicht alles als eine Reihe.
         * @param distance Die Distanz die geschoben werden soll
         * @return ByteRow Die resultierende ByteRow
         */
        virtual ByteRow operator>>(unsigned int distance) const noexcept {
            ByteRow<SIZE> block = ByteRow<SIZE>();
            for(unsigned int i = 0; i < SIZE; i++) {
                block[i] = (this->at(i))<<distance;
            }
            return block;
        }

        /**
         * \~english
         * @brief Moves all data of the ByteRow to the down by one.
         * @return ByteRow the resulting ByteRow
         *
         * \~german
         * @brief Schiebt die Daten um 1 Distanz nach unten.
         * @return ByteRow Die resultierende ByteRow
         */
        virtual ByteRow operator--() const noexcept {
            ByteRow<SIZE> block = ByteRow<SIZE>();
            for(unsigned int i = 0; i < SIZE-1; i++) {
                block[i] = (this->at(i+1));
            }
            return block;
        }

        /**
         * \~english
         * @brief Moves all data of the ByteRow to the up by one.
         * @return ByteRow the resulting ByteRow
         *
         * \~german
         * @brief Schiebt die Daten um 1 Distanz nach oben.
         * @return ByteRow Die resultierende ByteRow
         */
        virtual ByteRow operator++() const noexcept {
            ByteRow<SIZE> block = ByteRow<SIZE>();
            for(unsigned int i = 1; i < SIZE; i++) {
                block[i] = (this->at(i-1));
            }
            return block;
        }

        /**
         * \~english
         * @brief checks if two byteRows have identical data
         *
         * @param other the other byteRow
         * @return true the data is equal
         * @return false the data is not equal
         *
         * \~german
         * @brief prüft ob zwei ByteRow objekte identische Daten haben
         *
         * @param other das andere byteRow Objekt
         * @return true die Daten sind identisch
         * @return false die Daten sind nicht identisch
         */
        virtual bool operator==(const ByteRow<SIZE>& other) const noexcept {
            for (size_t i = 0; i < SIZE; i++) {
                if(this->at(i) != other.at(i)) {
                    return false;
                };
            }
            return true;
        }

        /**
                 * @brief checks if two byteRows do not have identical data
                 *
                 * @param other the other byteRow
                 * @return true the data is not equal
                 * @return false the data is equal
                 *
                 * \~german
                 * @brief prüft ob zwei ByteRow objekte nicht identische Daten haben
                 *
                 * @param other das andere byteRow Objekt
                 * @return true die Daten sind nicht identisch
                 * @return false die Daten sind identisch
                 */
        virtual bool operator!=(const ByteRow<SIZE>& other) const noexcept {
            for (size_t i = 0; i < SIZE; i++) {
                if(this->at(i) == other.at(i)) {
                    return false;
                };
            }
            return true;
        }

    };

}
