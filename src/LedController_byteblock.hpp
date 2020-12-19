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

class ByteBlock{
protected:
    byte _data [8] = {0,0,0,0,0,0,0,0};
public:
    ByteBlock(byte newdata[8]){
        for(uint8_t i = 0;i < 8;i++){
            _data[i] = newdata[i];
        }
    }
    ByteBlock(){};
    ~ByteBlock(){}

    byte& operator[] (uint8_t index){
        index %= 8;
        return _data[index];
    }
};
