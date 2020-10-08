#include "Move.hpp"

#include <iostream>

static byte in_array[4] = {
    B00000001,
    B00001000,
    B00100000,
    B10000000
};

void test_left_right(controller_configuration& conf){
    try{
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

void test_right_left(controller_configuration& conf){
    try{
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

void test_up_down(controller_configuration& conf){
    try{
        LedController control = LedController(conf);

        control.moveUp(in_array,nullptr);
        byte* out = new byte[4];
        control.moveDown(in_array,&out);
        for(unsigned int i = 0; i < 4;i++){
            TEST_ASSERT_EQUAL(in_array[i],out[i]);
        }
        delete out;
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

void test_down_up(controller_configuration& conf){
    try{
        LedController control = LedController(conf);

        control.moveDown(in_array,nullptr);
        byte* out = new byte[4];
        control.moveUp(in_array,&out);
        for(unsigned int i = 0; i < 4;i++){
            TEST_ASSERT_EQUAL(in_array[i],out[i]);
        }
        delete out;
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

static void test_left_right_default(){
    test_left_right(get_conf());
}
static void test_right_left_default(){
    test_right_left(get_conf());
}
static void test_up_down_default(){
    test_up_down(get_conf());
}
static void test_down_up_default(){
    test_down_up(get_conf());
}

static void test_left_right_SPI(){
    test_left_right(get_conf_SPI());
}
static void test_right_left_SPI(){
    test_right_left(get_conf_SPI());
}
static void test_up_down_SPI(){
    test_up_down(get_conf_SPI());
}
static void test_down_up_SPI(){
    test_down_up(get_conf_SPI());
}

void run_move_tests(){
    RUN_TEST(test_left_right_default);
    RUN_TEST(test_right_left_default);
    RUN_TEST(test_up_down_default);
    RUN_TEST(test_down_up_default);
    
    RUN_TEST(test_left_right_SPI);
    RUN_TEST(test_right_left_SPI);
    RUN_TEST(test_up_down_SPI);
    RUN_TEST(test_down_up_SPI);
}