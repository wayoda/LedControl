#pragma once

#define SPI_OUTPUT
#include "LedController.hpp"
#include <unity.h>

#define CATCH_STD catch(const std::exception& e){const char* msg = e.what(); TEST_FAIL_MESSAGE(msg); }

#ifndef ARDUINO
    using namespace fakeit;
    #define CATCH_FAKEIT catch(const FakeitException& e){std::cout << e.what() << std::endl; const char* msg = e.what().c_str(); TEST_FAIL_MESSAGE(msg);}
#endif

controller_configuration& get_conf();
controller_configuration& get_conf_SPI();
