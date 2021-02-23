#include "Reverse.hpp"

void test_reverse_simple(){
    LedController<1,1> control = LedController<1,1>();
    TEST_ASSERT_EQUAL(0x01, control.reverse(0x80));
}

void reverse_Block(){
    LedController<1,1> control = LedController<1,1>();

    ByteBlock b = {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};
    ByteBlock c = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};

    TEST_ASSERT(c == control.reverse(b));
}