#include "Reverse.hpp"

void test_reverse_simple(){
    LedController<1,1> control = LedController<1,1>();
    TEST_ASSERT_EQUAL(0x01, control.reverse(0x80));
}

void reverse_Block(){
    LedController<1,1> control = LedController<1,1>();

    ByteBlock b = {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};
    ByteBlock c = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
    ByteBlock d = control.reverse(b);

    for (unsigned int i = 0; i < 8;i++){
        TEST_ASSERT_EQUAL(c[i], d[i]);
    }
}