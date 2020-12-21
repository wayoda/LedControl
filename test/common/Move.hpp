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

        auto in = in_array<rows>();
        auto out = ByteRow<rows>();

        control.moveLeft(in,nullptr);
        control.moveRight(in,&out);
        for(unsigned int i = 0; i < rows;i++){
            TEST_ASSERT_EQUAL(in[i],out[i]);
        }
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
        auto out = ByteRow<rows>();

        control.moveRight(in,nullptr);
        control.moveLeft(in,&out);
        for(unsigned int i = 0; i < rows;i++){
            TEST_ASSERT_EQUAL(in[i],out[i]);
        }
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
        
        ByteRow<columns> in = in_array<columns>();
        ByteRow<columns> out = ByteRow<columns>();

        control.moveUp(in,nullptr);
        control.moveDown(in,&out);

        for(unsigned int i = 0; i < columns;i++){
            TEST_ASSERT_EQUAL(in[i],out[i]);
        }
        
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

        ByteRow<columns> in = in_array<columns>();
        ByteRow<columns> out = ByteRow<columns>();

        control.moveDown(in,nullptr);
        control.moveUp(in,&out);

        for(unsigned int i = 0; i < columns;i++){
            TEST_ASSERT_EQUAL(in[i],out[i]);
        }
        
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}
