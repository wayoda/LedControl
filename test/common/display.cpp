#include "display.hpp"

static void test_setColumn() {
    auto lc = LedController<4,1>(get_conf_SPI());

    lc.setColumn(0,7,B11111111);
    sakurajin::ByteBlock r = ByteBlock::rotate180({B00000001,B00000001,B00000001,B00000001,B00000001,B00000001,B00000001,B00000001});

    lc.setColumn(1,0,B11111111);
    sakurajin::ByteBlock s = {B00000001,B00000001,B00000001,B00000001,B00000001,B00000001,B00000001,B00000001};

    TEST_ASSERT(lc.getSegmentData(0) == r);
    TEST_ASSERT(lc.getSegmentData(1) == s);
}

static void test_setLED(){
    auto lc = LedController<4,1>(get_conf_SPI());

    lc.setLed(0,0,0,true);
    sakurajin::ByteBlock a = ByteBlock::rotate180({B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B10000000});

    lc.setLed(1,0,4,true);
    sakurajin::ByteBlock b = ByteBlock::rotate180({B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001000});

    lc.setLed(2,2,0,true);
    sakurajin::ByteBlock c = ByteBlock::rotate180({B00000000,B00000000,B00000000,B00000000,B00000000,B10000000,B00000000,B00000000});

    TEST_ASSERT(lc.getSegmentData(0) == a);
    TEST_ASSERT(lc.getSegmentData(1) == b);
    TEST_ASSERT(lc.getSegmentData(2) == c);

}

void run_display_tests(){
    RUN_TEST(test_setColumn);
    RUN_TEST(test_setLED);
}