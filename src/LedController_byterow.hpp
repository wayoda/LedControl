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

template<size_t SIZE>
class ByteRow{
    protected:
        byte _data[SIZE];
    public:
        ByteRow(byte newdata[SIZE]):ByteRow(){
            for(uint64_t i = 0;i < SIZE;i++){
                _data[i] = newdata[i];
            }
        };
        ByteRow(){
            for(uint64_t i = 0; i < SIZE;i++){
                _data[i] = 0;
            }
        };
        ~ByteRow(){};

        byte& operator[] (uint8_t index){
            index %= SIZE;
            return _data[index];
        }

        const byte& operator[] (uint8_t index) const{
            index %= SIZE;
            return _data[index];
        }

        ByteRow& operator= (byte newdata[SIZE]){
            for(uint8_t i = 0;i < SIZE;i++){
                _data[i] = newdata[i];
            }
            return *this;
        }

};
