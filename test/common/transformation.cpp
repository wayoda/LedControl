#include "transformation.hpp"

static void test_reverse_simple() {
    TEST_ASSERT_EQUAL(0x01, ByteBlock::reverse(0x80));
}

static void test_reverse_Block() {
    ByteBlock b = {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};
    ByteBlock c = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};

    TEST_ASSERT(c == b.reverse());
    TEST_ASSERT(b == c.reverse());
    TEST_ASSERT(testBlock() == testBlock().reverse().reverse() );
}

static void test_makeColumns() {
    ByteBlock c = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
    ByteBlock d = {B11111111,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    TEST_ASSERT(d == c.makeColumns());
    TEST_ASSERT(c == d.makeColumns());
    TEST_ASSERT(c == c.makeColumns().makeColumns())
    TEST_ASSERT(d == d.makeColumns().makeColumns())
}

static void test_rotate180(){
    ByteBlock a = {0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    ByteBlock b = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};

    TEST_ASSERT(a == b.rotate180());
    TEST_ASSERT(b == a.rotate180());
    TEST_ASSERT(a == a.rotate180().rotate180());
}

//run all of the tests defined above
void run_transformation_tests() {
    RUN_TEST(test_reverse_simple);
    RUN_TEST(test_reverse_Block);
    RUN_TEST(test_makeColumns);
    RUN_TEST(test_rotate180);
}
