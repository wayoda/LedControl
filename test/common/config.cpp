#include "config.hpp"

static void check_valid_configs() {
    TEST_ASSERT(get_conf().isValid());
    TEST_ASSERT(get_conf_SPI().isValid());
    TEST_ASSERT(get_multi_conf().isValid());
    TEST_ASSERT(get_multi_conf_SPI().isValid());
}

static void check_invalid_configs() {

    sakurajin::controller_configuration<1,2> c1;
    TEST_ASSERT(!c1.isValid());

    c1.useHardwareSpi = true;
    c1.virtual_multi_row = false;
    TEST_ASSERT(!c1.isValid());

    c1.row_SPI_CS[0] = 1;
    TEST_ASSERT(!c1.isValid());

    c1.row_SPI_CS[1] = 1;
    TEST_ASSERT(!c1.isValid());

    c1.row_SPI_CS[1] = 2;
    TEST_ASSERT(c1.isValid());

}

void run_config_tests() {
    RUN_TEST(check_valid_configs);
    RUN_TEST(check_invalid_configs);
}