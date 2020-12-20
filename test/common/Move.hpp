#pragma once
#include "common.hpp"

template <size_t columns, size_t rows>
void test_left_right(controller_configuration<columns,rows>& conf);

template <size_t columns, size_t rows>
void test_right_left(controller_configuration<columns,rows>& conf);

template <size_t columns, size_t rows>
void test_up_down(controller_configuration<columns,rows>& conf);

template <size_t columns, size_t rows>
void test_down_up(controller_configuration<columns,rows>& conf);

void run_move_tests();

//function definitions
template <size_t columns, size_t rows>
void test_left_right(controller_configuration<columns,rows>& conf){
    try{
        LedController<columns,rows> control = LedController<columns,rows>(conf);

        auto in = B00000101;
        control.moveLeft(in);
        TEST_ASSERT_EQUAL(in, control.moveRight(0));
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

template <size_t columns, size_t rows>
void test_right_left(controller_configuration<columns,rows>& conf){
    try{
        LedController<columns,rows> control = LedController<columns,rows>(conf);

        auto in = B00000101;
        control.moveRight(in);
        TEST_ASSERT_EQUAL(in, control.moveLeft(0));
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

template <size_t columns, size_t rows>
void test_up_down(controller_configuration<columns,rows>& conf){
    try{
        LedController<columns,rows> control = LedController<columns,rows>(conf);

        control.moveUp(in_array(),nullptr);
        byte* out = new byte[4];
        control.moveDown(in_array(),&out);
        for(unsigned int i = 0; i < 4;i++){
            TEST_ASSERT_EQUAL(in_array()[i],out[i]);
        }
        delete out;
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

template <size_t columns, size_t rows>
void test_down_up(controller_configuration<columns,rows>& conf){
    try{
        LedController<columns,rows> control = LedController<columns,rows>(conf);

        control.moveDown(in_array(),nullptr);
        byte* out = new byte[4];
        control.moveUp(in_array(),&out);
        for(unsigned int i = 0; i < 4;i++){
            TEST_ASSERT_EQUAL(in_array()[i],out[i]);
        }
        delete out;
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}
