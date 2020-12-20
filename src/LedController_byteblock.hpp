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

class ByteBlock : public ByteRow<8>{
public:
    ByteBlock(byte newdata[8]):ByteRow<8>(newdata){}
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
    ByteBlock():ByteRow<8>(){};
};
