#include "common.hpp"

const sakurajin::ByteBlock& testBlock() {
    static auto dat = sakurajin::ByteBlock({0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80});
    return dat;
}

sakurajin::controller_configuration<4,1>& get_conf() {
    static sakurajin::controller_configuration<4,1> conf;
    if(!conf.isValid()) {
        conf.SPI_CS = 16;
        conf.SPI_CLK = 15;
        conf.SPI_MOSI = 14;
        conf.useHardwareSpi = false;
        conf.isValid();
    }

    return conf;
}

sakurajin::controller_configuration<4,1>& get_conf_SPI() {
    static sakurajin::controller_configuration<4,1> conf;
    if(!conf.isValid()) {
        conf.SPI_CS = 16;
        conf.useHardwareSpi = true;
        conf.isValid();
    }
    return conf;
}


sakurajin::controller_configuration<4,4>& get_multi_conf() {
    static sakurajin::controller_configuration<4,4> conf;
    if(!conf.isValid()) {
        conf.SPI_CLK = 15;
        conf.SPI_MOSI = 14;
        conf.SPI_CS = 13;
        conf.useHardwareSpi = false;
        conf.isValid();
    }

    return conf;
}

sakurajin::controller_configuration<4,4>& get_multi_conf_SPI() {
    static sakurajin::controller_configuration<4,4> conf;
    if(!conf.isValid()) {
        conf.SPI_CS = 13;
        conf.useHardwareSpi = true;
        conf.isValid();
    }

    return conf;
}
