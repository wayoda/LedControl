#include "LedController.hpp"

template <size_t columns, size_t rows>
sakurajin::LedController<columns,rows>::~LedController() noexcept {
    initilized = false;
}

template <size_t columns, size_t rows>
sakurajin::LedController<columns,rows>::LedController() noexcept {};

template <size_t columns, size_t rows>
sakurajin::LedController<columns,rows>::LedController(unsigned int csPin) noexcept {
    init(csPin);
};

template <size_t columns, size_t rows>
sakurajin::LedController<columns,rows>::LedController(unsigned int dataPin, unsigned int clkPin,
        unsigned int csPin, bool useHardwareSpiParam) noexcept {
    init(dataPin, clkPin, csPin, useHardwareSpiParam);
}

template <size_t columns, size_t rows>
sakurajin::LedController<columns,rows>::LedController(const sakurajin::controller_configuration<columns,rows> &config) noexcept {
    init(config);
};

template <size_t columns, size_t rows>
sakurajin::LedController<columns,rows>::LedController(const sakurajin::LedController<columns,rows> &other) noexcept {
    if (!other.initilized) {
        return;
    }

    init(other.conf);

    for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
        for (unsigned int j = 0; i < 8; i++) {
            LedStates[i][j] = other.LedStates[i][j];
        }
    }

    for (unsigned int j = 0; j < rows*columns*2; j++) {
        spidata[j] = other.spidata[j];
    }

    refreshSegments();
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::init(unsigned int csPin) noexcept {
    sakurajin::controller_configuration<columns,rows> config;

    config.SPI_CS = csPin;
    config.useHardwareSpi = true;

    return init(config);
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::init(unsigned int dataPin, unsigned int clkPin,
        unsigned int csPin, bool useHardwareSpiParam) noexcept {
    if (initilized) {
        return;
    }

    sakurajin::controller_configuration<columns,rows> config;

    config.SPI_MOSI = dataPin;
    config.SPI_CLK = clkPin;
    config.SPI_CS = csPin;
    config.useHardwareSpi = useHardwareSpiParam;

    init(config);
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::init(const sakurajin::controller_configuration<columns,rows> &configuration) noexcept {
    if (initilized) {
        return;
    }

    if (!configuration.isValid()) {
        return;
    }
    conf = configuration;

    initConf();
    initSPI();

    initilized = true;
    refreshSegments();
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::initConf() noexcept {
    if (conf.useHardwareSpi) {
        conf.SPI_CLK = SCK;
        conf.SPI_MOSI = MOSI;
    }

    resetBuffers();
}

template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::initSPI() noexcept {
    pinMode(conf.SPI_MOSI, OUTPUT);
    pinMode(conf.SPI_CLK, OUTPUT);

    if(conf.virtual_multi_row) {
        pinMode(conf.SPI_CS,OUTPUT);
        digitalWrite(conf.SPI_CS,LOW);
    } else {
        for(unsigned int i = 0; i < rows; i++) {
            pinMode(conf.row_SPI_CS[i],OUTPUT);
            digitalWrite(conf.row_SPI_CS[i],LOW);
        }
    }

    if (conf.useHardwareSpi) {
        SPI.setBitOrder(MSBFIRST);
        SPI.setDataMode(SPI_MODE0);
        SPI.begin();
    }

    if(conf.virtual_multi_row) {
        digitalWrite(conf.SPI_CS,HIGH);
    } else {
        for(unsigned int i = 0; i < rows; i++) {
            digitalWrite(conf.row_SPI_CS[i],HIGH);
        }
    }
}

template <size_t columns, size_t rows>
bool sakurajin::LedController<columns,rows>::isInitilized() noexcept {
    return initilized;
}

// to be remvoed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::getSegmentData(unsigned int column, unsigned int row_num, sakurajin::ByteBlock* resultLocation) noexcept {
    getSegmentData(conf.getSegmentNumber(column,row_num),resultLocation);
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void sakurajin::LedController<columns,rows>::getSegmentData(unsigned int segmentindex,
        sakurajin::ByteBlock *resultLocation) noexcept {
    if (!initilized || segmentindex >= conf.SegmentCount() ||
            resultLocation == nullptr) {
        return;
    }

    *resultLocation = getSegmentData(segmentindex);
}

template <size_t columns, size_t rows>
sakurajin::ByteBlock sakurajin::LedController<columns,rows>::getSegmentData(unsigned int segmentindex) noexcept {
    if (!initilized || segmentindex >= conf.SegmentCount()) {
        return sakurajin::ByteBlock();
    };
    return LedStates[segmentindex];
}

template <size_t columns, size_t rows>
sakurajin::ByteBlock sakurajin::LedController<columns,rows>::getSegmentData(unsigned int column, unsigned int row_num) noexcept {
    return getSegmentData(conf.getSegmentNumber(column,row_num));
}

template <size_t columns, size_t rows>
unsigned int sakurajin::LedController<columns,rows>::getSegmentCount() noexcept {
    return columns*rows;
}

template <size_t columns, size_t rows>
const sakurajin::controller_configuration<columns,rows>& sakurajin::LedController<columns,rows>::getConfig() noexcept {
    return conf;
}
