#include "Move.hpp"

#include <iostream>

controller_configuration& get_conf(){
    static controller_configuration conf;
    if(!conf.isValid()){
        PRINTLN("expected fail message. Creating default configuration.");
        conf.SegmentCount = 4;
        conf.SPI_CS = 16;
        conf.SPI_CLK = 15;
        conf.SPI_MOSI = 14;
        conf.row_SPI_CS = nullptr;
        conf.rows = 1;
        conf.useHardwareSpi = false;
    }

    return conf;
}

void test_left_right(){
    try{
        controller_configuration conf = get_conf();
        TEST_ASSERT_TRUE(conf.isValid());

        LedController control = LedController(conf);

        auto in = B00000101;
        control.moveLeft(in);
        TEST_ASSERT_EQUAL(in, control.moveRight(0));
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

void test_right_left(){
    try{
        controller_configuration conf = get_conf();
        TEST_ASSERT_TRUE(conf.isValid());

        LedController control = LedController(conf);

        auto in = B00000101;
        control.moveRight(in);
        TEST_ASSERT_EQUAL(in, control.moveLeft(0));
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

void test_up_down(){
    try{
        controller_configuration conf = get_conf();
        LedController control = LedController(conf);

        byte in[4];
        in[0] = B00000001;
        in[1] = B00001000;
        in[2] = B00100000;
        in[3] = B10000000;
        control.moveUp(in,nullptr);
        byte* out = new byte[4];
        control.moveDown(in,&out);
        for(unsigned int i = 0; i < 4;i++){
            TEST_ASSERT_EQUAL(in[i],out[i]);
        }
        delete out;
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

void test_down_up(){
    try{
        controller_configuration conf = get_conf();
        LedController control = LedController(conf);

        byte in[4];
        in[0] = B00000001;
        in[1] = B00001000;
        in[2] = B00100000;
        in[3] = B10000000;
        control.moveDown(in,nullptr);
        byte* out = new byte[4];
        control.moveUp(in,&out);
        for(unsigned int i = 0; i < 4;i++){
            TEST_ASSERT_EQUAL(in[i],out[i]);
        }
        delete out;
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}
