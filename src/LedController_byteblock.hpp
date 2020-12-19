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
    ByteBlock(byte n0,byte n1,byte n2,byte n3,byte n4,byte n5,byte n6,byte n7){
        _data[0] = n0;
        _data[1] = n1;
        _data[2] = n2;
        _data[3] = n3;
        _data[4] = n4;
        _data[5] = n5;
        _data[6] = n6;
        _data[7] = n7;
    };
    ByteBlock(){};
    ~ByteBlock(){}

    byte& operator[] (uint8_t index){
        index %= 8;
        return _data[index];
    }

    ByteBlock& operator= (byte newdata[8]){
        for(uint8_t i = 0;i < 8;i++){
            _data[i] = newdata[i];
        }
        return *this;
    }
};
