#include "common.hpp"

controller_configuration<4,1>& get_conf(){
    static controller_configuration<4,1> conf;
    if(!conf.isValid()){
        conf.SPI_CS = 16;
        conf.SPI_CLK = 15;
        conf.SPI_MOSI = 14;
        conf.useHardwareSpi = false;
        conf.isValid();
    }

    return conf;
}

controller_configuration<4,1>& get_conf_SPI(){
    static controller_configuration<4,1> conf;
    if(!conf.isValid()){
        conf.SPI_CS = 16;
        conf.useHardwareSpi = true;
        conf.isValid();
    }
    return conf;
}


controller_configuration<4,4>& get_multi_conf(){
    static controller_configuration<4,4> conf;
    if(!conf.isValid()){
        conf.SPI_CLK = 15;
        conf.SPI_MOSI = 14;      
        conf.SPI_CS = 13;
        conf.useHardwareSpi = false;
        conf.isValid();
    }

    return conf;
}

controller_configuration<4,4>& get_multi_conf_SPI(){
    static controller_configuration<4,4> conf;
    if(!conf.isValid()){
        conf.SPI_CS = 13;
        conf.useHardwareSpi = true;
        conf.isValid();
    }
    
    return conf;
}
