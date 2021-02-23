#include "transformation.hpp"

void run_transformation_tests(){
    RUN_TEST(test_reverse_simple);
    RUN_TEST(reverse_Block);
}

void test_reverse_simple(){
    LedController<1,1> control = LedController<1,1>();
    TEST_ASSERT_EQUAL(0x01, control.reverse(0x80));
}

void reverse_Block(){
    LedController<1,1> control = LedController<1,1>();

    ByteBlock b = {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};
    ByteBlock c = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};

    TEST_ASSERT(c == control.reverse(b));
    TEST_ASSERT(b == control.reverse(c));
    TEST_ASSERT(testBlock() == control.reverse( control.reverse( testBlock() ) ));
}
