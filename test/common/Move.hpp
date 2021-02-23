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
void run_move_multi_tests();

//function definitions
template <size_t columns, size_t rows>
void test_left_right(controller_configuration<columns,rows>& conf){
    try{
        LedController<columns,rows> control = LedController<columns,rows>(conf);

        auto in = in_array<rows>();
        control.moveLeft(in);
        auto out = control.moveRight();

        TEST_ASSERT(in == out);
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

        auto in = in_array<rows>();
        control.moveRight(in);
        auto out = control.moveLeft();

        TEST_ASSERT(in == out);
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
        
        auto in = in_array<columns>();
        control.moveUp(in);
        auto out = control.moveDown();

        TEST_ASSERT(in == out);
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

        auto in = in_array<columns>();
        control.moveDown(in);
        auto out = control.moveUp();

        TEST_ASSERT(in == out);
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}
