#include "Move.hpp"

#include <iostream>

//simple movement tests
static void test_left_right_default() {
    test_left_right<4,1>(get_conf());
}
static void test_right_left_default() {
    test_right_left<4,1>(get_conf());
}
static void test_up_down_default() {
    test_up_down<4,1>(get_conf());
}
static void test_down_up_default() {
    test_down_up<4,1>(get_conf());
}

static void test_left_right_SPI() {
    test_left_right<4,1>(get_conf_SPI());
}
static void test_right_left_SPI() {
    test_right_left<4,1>(get_conf_SPI());
}
static void test_up_down_SPI() {
    test_up_down<4,1>(get_conf_SPI());
}
static void test_down_up_SPI() {
    test_down_up<4,1>(get_conf_SPI());
}

static void test_left_right_default_multi() {
    test_left_right<4,4>(get_multi_conf());
}
static void test_right_left_default_multi() {
    test_right_left<4,4>(get_multi_conf());
}
static void test_up_down_default_multi() {
    test_up_down<4,4>(get_multi_conf());
}
static void test_down_up_default_multi() {
    test_down_up<4,4>(get_multi_conf());
}

static void test_left_right_SPI_multi() {
    test_left_right<4,4>(get_multi_conf_SPI());
}
static void test_right_left_SPI_multi() {
    test_right_left<4,4>(get_multi_conf_SPI());
}
static void test_up_down_SPI_multi() {
    test_up_down<4,4>(get_multi_conf_SPI());
}
static void test_down_up_SPI_multi() {
    test_down_up<4,4>(get_multi_conf_SPI());
}

//move a byte block in and check if that was correct
static void test_full_block() {
    auto lc = LedController<4,1>(get_conf_SPI());

    for (unsigned int i = 0; i < 8; i++) {
        lc.moveRowRight(testBlock().transpose().at(7-i));
    }
    TEST_ASSERT(lc.getSegmentData(0) == testBlock());

    for (unsigned int i = 0; i < 8; i++) {
        lc.moveRowLeft(testBlock().transpose().at(i));
    }
    TEST_ASSERT(lc.getSegmentData(3) == testBlock());

    for (unsigned int i = 0; i < 8; i++) {
        lc.moveColumnUp(testBlock().at(7-i));
    }
    TEST_ASSERT(lc.getSegmentData(0) == testBlock());

    for (unsigned int i = 0; i < 8; i++) {
        lc.moveColumnDown(testBlock().at(i));
    }
    TEST_ASSERT(lc.getSegmentData(0) == testBlock());

}

void run_move_tests() {
    RUN_TEST(test_left_right_default_multi);
    RUN_TEST(test_right_left_default_multi);
    RUN_TEST(test_up_down_default_multi);
    RUN_TEST(test_down_up_default_multi);

    RUN_TEST(test_left_right_SPI_multi);
    RUN_TEST(test_right_left_SPI_multi);
    RUN_TEST(test_up_down_SPI_multi);
    RUN_TEST(test_down_up_SPI_multi);

    RUN_TEST(test_left_right_default);
    RUN_TEST(test_right_left_default);
    RUN_TEST(test_up_down_default);
    RUN_TEST(test_down_up_default);

    RUN_TEST(test_left_right_SPI);
    RUN_TEST(test_right_left_SPI);
    RUN_TEST(test_up_down_SPI);
    RUN_TEST(test_down_up_SPI);

    RUN_TEST(test_full_block);
}
