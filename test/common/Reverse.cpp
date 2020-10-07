#include "Reverse.hpp"

void test_reverse_simple(){
    LedController control = LedController();
    TEST_ASSERT_EQUAL(0x01, control.reverse(0x80));
}

void reverse_Block(){
    LedController control = LedController();

    ByteBlock b = {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};
    ByteBlock c = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
    ByteBlock d;
    control.reverse(b, &d);

    for (unsigned int i = 0; i < 8;i++){
        TEST_ASSERT_EQUAL(c[i], d[i]);
        TEST_ASSERT_EQUAL(d[i], c[i]);
    }
}