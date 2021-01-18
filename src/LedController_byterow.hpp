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
class ByteRow{
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
        ByteRow(byte newdata[SIZE]):ByteRow(){
            for(uint64_t i = 0;i < SIZE;i++){
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
        ByteRow(){
            for(uint64_t i = 0; i < SIZE;i++){
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
        ~ByteRow(){};

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
        byte& operator[] (uint64_t index){
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
        const byte& operator[] (uint64_t index) const{
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
        byte at(uint64_t index) const{
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
        ByteRow& operator= (byte newdata[SIZE]){
            for(uint64_t i = 0;i < SIZE;i++){
                _data[i] = newdata[i];
            }
            return *this;
        }

};
