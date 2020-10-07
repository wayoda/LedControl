#include "common.hpp"

controller_configuration& get_conf(){
    static controller_configuration conf;
    if(!conf.isValid()){
        PRINTLN("expected fail message. Creating default configuration.");
        conf.SegmentCount = 4;
        conf.SPI_CS = 16;
        conf.SPI_CLK = 15;
        conf.SPI_MOSI = 14;
        conf.row_SPI_CS = nullptr;
        conf.rows = 1;
        conf.useHardwareSpi = false;
    }

    return conf;
}

controller_configuration& get_conf_SPI(){
    static controller_configuration conf;
    if(!conf.isValid()){
        PRINTLN("expected fail message. Creating default configuration.");
        conf.SegmentCount = 4;
        conf.SPI_CS = 16;
        conf.rows = 1;
        conf.useHardwareSpi = true;
    }
    return conf;
}