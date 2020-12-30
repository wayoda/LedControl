#include "Move.hpp"

#include <iostream>

static void test_left_right_default(){
    test_left_right<4,1>(get_conf());
}
static void test_right_left_default(){
    test_right_left<4,1>(get_conf());
}
static void test_up_down_default(){
    test_up_down<4,1>(get_conf());
}
static void test_down_up_default(){
    test_down_up<4,1>(get_conf());
}

static void test_left_right_SPI(){
    test_left_right<4,1>(get_conf_SPI());
}
static void test_right_left_SPI(){
    test_right_left<4,1>(get_conf_SPI());
}
static void test_up_down_SPI(){
    test_up_down<4,1>(get_conf_SPI());
}
static void test_down_up_SPI(){
    test_down_up<4,1>(get_conf_SPI());
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

static void test_left_right_default_multi(){
    test_left_right<4,4>(get_multi_conf());
}
static void test_right_left_default_multi(){
    test_right_left<4,4>(get_multi_conf());
}
static void test_up_down_default_multi(){
    test_up_down<4,4>(get_multi_conf());
}
static void test_down_up_default_multi(){
    test_down_up<4,4>(get_multi_conf());
}

static void test_left_right_SPI_multi(){
    test_left_right<4,4>(get_multi_conf_SPI());
}
static void test_right_left_SPI_multi(){
    test_right_left<4,4>(get_multi_conf_SPI());
}
static void test_up_down_SPI_multi(){
    test_up_down<4,4>(get_multi_conf_SPI());
}
static void test_down_up_SPI_multi(){
    test_down_up<4,4>(get_multi_conf_SPI());
}

void run_move_multi_tests(){
    RUN_TEST(test_left_right_default_multi);
    RUN_TEST(test_right_left_default_multi);
    RUN_TEST(test_up_down_default_multi);
    RUN_TEST(test_down_up_default_multi);
    
    RUN_TEST(test_left_right_SPI_multi);
    RUN_TEST(test_right_left_SPI_multi);
    RUN_TEST(test_up_down_SPI_multi);
    RUN_TEST(test_down_up_SPI_multi);
}