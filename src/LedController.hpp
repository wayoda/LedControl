#pragma once
#include "LedController_template.hpp"
#include "LedController_movement.hpp"

// the opcodes for the MAX7221 and MAX7219
#define OP_NOOP 0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE 9
#define OP_INTENSITY 10
#define OP_SCANLIMIT 11
#define OP_SHUTDOWN 12
#define OP_DISPLAYTEST 15

template <size_t columns, size_t rows>
LedController<columns,rows>::~LedController() {
  initilized = false;
}

template <size_t columns, size_t rows>
LedController<columns,rows>::LedController(){};

template <size_t columns, size_t rows>
LedController<columns,rows>::LedController(unsigned int csPin) {
  init(csPin);
};

template <size_t columns, size_t rows>
LedController<columns,rows>::LedController(unsigned int dataPin, unsigned int clkPin,
                             unsigned int csPin, bool useHardwareSpiParam) {
  init(dataPin, clkPin, csPin, useHardwareSpiParam);
}

template <size_t columns, size_t rows>
LedController<columns,rows>::LedController(const controller_configuration<columns,rows> &config) {
  init(config);
};

template <size_t columns, size_t rows>
LedController<columns,rows>::LedController(const LedController &other) {
  if (!other.initilized) {
    return;
  }

  init(other.conf);

  for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
    for (unsigned int j = 0; i < 8; i++) {
      LedStates[i][j] = other.LedStates[i][j];
    }
  }

  for (unsigned int j = 0; j < rows;j++){
    for (unsigned int i = 0; i < columns*2; i++) {
      spidata[j][i] = other.spidata[j][i];
    }
  }

  refreshSegments();
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::init(unsigned int csPin) {
  controller_configuration<columns,rows> config;

  config.SPI_CS = csPin;
  config.useHardwareSpi = true;

  return init(config);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::init(unsigned int dataPin, unsigned int clkPin,
                         unsigned int csPin, bool useHardwareSpiParam) {
  if (initilized) {
    return;
  }

  controller_configuration<columns,rows> config;

  config.SPI_MOSI = dataPin;
  config.SPI_CLK = clkPin;
  config.SPI_CS = csPin;
  config.useHardwareSpi = useHardwareSpiParam;

  init(config);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::init(const controller_configuration<columns,rows> &configuration) {
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
void LedController<columns,rows>::initConf(){
  if (conf.useHardwareSpi) {
    conf.SPI_CLK = SCK;
    conf.SPI_MOSI = MOSI;
  }

  resetBuffers();
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::initSPI(){
  pinMode(conf.SPI_MOSI, OUTPUT);
  pinMode(conf.SPI_CLK, OUTPUT);
 
  if(conf.virtual_multi_row){
    pinMode(conf.SPI_CS,OUTPUT);
    digitalWrite(conf.SPI_CS,LOW);
  }else{
    for(unsigned int i = 0; i < rows;i++){
      pinMode(conf.row_SPI_CS[i],OUTPUT);
      digitalWrite(conf.row_SPI_CS[i],LOW);
    }
  }

  if (conf.useHardwareSpi) {
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.begin();
  }

  if(conf.virtual_multi_row){
    digitalWrite(conf.SPI_CS,HIGH);
  }else{
    for(unsigned int i = 0; i < rows;i++){
      digitalWrite(conf.row_SPI_CS[i],HIGH);
    }
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::resetBuffers(){
  for (unsigned int j = 0; j < rows;j++){
    for (unsigned int i = 0; i < columns*2; i++) {
      spidata[j][i] = 0;
    }
  }

  for (unsigned int j = 0; j < conf.SegmentCount(); j++) {
    for (unsigned int i = 0; i < 8; i++) {
      LedStates[j][i] = 0x00;
    }
  }
}

template <size_t columns, size_t rows>
bool LedController<columns,rows>::isInitilized() { return initilized; }

template <size_t columns, size_t rows>
void LedController<columns,rows>::resetMatrix() {
  if (!initilized) {
    return;
  }

  activateAllSegments();
  setIntensity(1);
  clearMatrix();
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::clearMatrix() {
  if (!initilized) {
    return;
  }

  for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
    clearSegment(i); // Clear Segments
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setIntensity(unsigned int newIntesityLevel) {
  if (newIntesityLevel > 15 || !initilized) {
    return;
  }

  // std::lock_guard<std::mutex> lock(mut_IntensityLevel);
  conf.IntensityLevel = newIntesityLevel;

  for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
    setIntensity(i, conf.IntensityLevel);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setIntensity(unsigned int segmentNumber,
                                 unsigned int newIntesityLevel) {
  if (newIntesityLevel > 15 || !initilized ||
      segmentNumber >= conf.SegmentCount()) {
    return;
  }

  spiTransfer(segmentNumber, OP_INTENSITY, newIntesityLevel);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::displayOnSegment(unsigned int segmentindex,
                                     ByteBlock data) {
  if (!initilized || segmentindex >= conf.SegmentCount()) {
    return;
  }

  for (int i = 0; i < 8; i++) {
    setRow(segmentindex, i, data[i]);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::displayOnSegment(unsigned int column, unsigned int row_num, ByteBlock data){
  displayOnSegment(conf.getSegmentNumber(column,row_num),data);
}

// to be remvoed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::getSegmentData(unsigned int column, unsigned int row_num, ByteBlock* resultLocation){
  getSegmentData(conf.getSegmentNumber(column,row_num),resultLocation);
}
// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::getSegmentData(unsigned int segmentindex,
                                   ByteBlock *resultLocation) {
  if (!initilized || segmentindex >= conf.SegmentCount() ||
      resultLocation == nullptr) {
    return;
  }

  for (unsigned int i = 0; i < 8; i++) {
    (*resultLocation)[i] = LedStates[segmentindex][i];
  }
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::getSegmentData(unsigned int segmentindex){
  if (!initilized || segmentindex >= conf.SegmentCount()){return ByteBlock();};
  return LedStates[segmentindex];
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::getSegmentData(unsigned int column, unsigned int row_num){
  return getSegmentData(conf.getSegmentNumber(column,row_num));
}

template <size_t columns, size_t rows>
unsigned int LedController<columns,rows>::getSegmentCount() {
  if (!initilized) {
    return 0;
  }

  return conf.SegmentCount();
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::shutdownSegment(unsigned int segmentNumber) {
  if (!initilized || segmentNumber >= conf.SegmentCount()) {
    return;
  }

  spiTransfer(segmentNumber, OP_SHUTDOWN, 0);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::activateSegment(unsigned int segmentNumber) {
  if (!initilized || segmentNumber >= conf.SegmentCount()) {
    return;
  }

  spiTransfer(segmentNumber, OP_SHUTDOWN, 1);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::shutdownAllSegments() {
  if (!initilized) {
    return;
  }

  for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
    shutdownSegment(i);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::activateAllSegments() {
  if (!initilized) {
    return;
  }

  for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
    activateSegment(i);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::spiTransfer(unsigned int segment, byte opcode, byte data) {
  if (!initilized || segment >= conf.SegmentCount()) {
    return;
  }

  // Create an array with the data to shift out
  unsigned int offset = conf.getColumn(segment) * 2;
  unsigned int row = conf.getRow(segment);
  unsigned int maxbytes = conf.SegmentCount() * 2;

  for (unsigned int j = 0; j < rows;j++){
    for (unsigned int i = 0; i < columns*2; i++) {
      spidata[j][i] = 0x00;
    }
  }

  // put our device data into the array
  spidata[row][offset + 1] = opcode;
  spidata[row][offset] = data;
  
  for(int r = 0; r < rows ;r++){

    //enable the line
    auto cs = conf.virtual_multi_row ? conf.SPI_CS : conf.row_SPI_CS[r];
    digitalWrite(cs, LOW);

    //init the spi transfer if hardware should be used
    if (conf.useHardwareSpi) {
      SPI.beginTransaction(SPISettings(conf.spiTransferSpeed, MSBFIRST, SPI_MODE0));
    }

    // Now shift out the data
    for (int i = maxbytes; i > 0; i--) {
      if (conf.useHardwareSpi) {
        SPI.transfer(spidata[r][i - 1]);
      } else {
        shiftOut(conf.SPI_MOSI, conf.SPI_CLK, MSBFIRST, spidata[r][i - 1]);
      }
    }

    //end the spi transfer if hardware should be used
    if (conf.useHardwareSpi) {
      SPI.endTransaction();
    }

    // latch the data onto the display
    digitalWrite(cs, HIGH);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setScanLimit(unsigned int segmentNumber,
                                 unsigned int limit) {
  if (!initilized || segmentNumber >= conf.SegmentCount()) {
    return;
  };
  if (limit < 8) {
    spiTransfer(segmentNumber, OP_SCANLIMIT, limit);
  };
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::clearSegment(unsigned int segmentNumber) {
  if (!initilized || segmentNumber >= conf.SegmentCount()) {
    return;
  }

  for (int i = 0; i < 8; i++) {
    LedStates[segmentNumber][i] = 0x00;
    spiTransfer(segmentNumber, i + 1, LedStates[segmentNumber][i]);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setRow(unsigned int segmentNumber, unsigned int row,
                           byte value) {
  if (!initilized || segmentNumber >= conf.SegmentCount() || row > 7 ||
      (conf.onlySendOnChange && LedStates[segmentNumber][row] == value)) {
    return;
  }

  LedStates[segmentNumber][row] = value;
  spiTransfer(segmentNumber, row + 1, LedStates[segmentNumber][row]);
}

template <size_t columns, size_t rows>
byte LedController<columns,rows>::getRow(unsigned int segmentNumber, unsigned int row) {
  if (!initilized || segmentNumber >= conf.SegmentCount() || row > 7) {
    return 0x00;
  }

  return LedStates[segmentNumber][row];
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setLed(unsigned int segmentNumber, unsigned int row,
                           unsigned int column, boolean state) {
  if (!initilized || row > 7 || column > 7 ||
      segmentNumber >= conf.SegmentCount()) {
    return;
  };

  byte val = B10000000 >> column;

  if (state)
    LedStates[segmentNumber][row] = LedStates[segmentNumber][row] | val;
  else {
    val = ~val;
    LedStates[segmentNumber][row] = LedStates[segmentNumber][row] & val;
  }
  spiTransfer(segmentNumber, row + 1, LedStates[segmentNumber][row]);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setColumn(unsigned int segmentNumber, unsigned int col,
                              byte value) {
  if (!initilized || segmentNumber >= conf.SegmentCount() || col > 7) {
    return;
  };

  byte val;

  for (int row = 0; row < 8; row++) {
    // val = value & (0x01 << row);
    val = value >> (7 - row);
    val &= 0x01;
    val <<= col;
    val |= LedStates[segmentNumber][row];
    setRow(segmentNumber, row, val);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setDigit(unsigned int segmentNumber, unsigned int digit,
                             byte value, boolean dp) {
  if (!initilized || segmentNumber >= conf.SegmentCount() || digit > 7 ||
      value > 15) {
    return;
  };

  byte v = pgm_read_byte_near(charTable + value);
  if (dp) {
    v |= B10000000;
  };
  LedStates[segmentNumber][digit] = v;
  spiTransfer(segmentNumber, digit + 1, v);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::setChar(unsigned int segmentNumber, unsigned int digit,
                            char value, boolean dp) {
  if (!initilized || segmentNumber >= conf.SegmentCount() || digit > 7) {
    return;
  };

  byte index = (byte)value;
  if (index > 127) {
    // no defined beyond index 127, so we use the space char
    index = 32;
  }

  byte v = pgm_read_byte_near(charTable + index);
  if (dp) {
    v |= B10000000;
  };

  LedStates[segmentNumber][digit] = v;
  spiTransfer(segmentNumber, digit + 1, v);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::refreshSegments() {
  if (!initilized) {
    return;
  }

  for (unsigned int i = 0; i < conf.SegmentCount(); i++) {
    refreshSegment(i);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::refreshSegment(unsigned int segmentNumber) {
  if (!initilized) {
    return;
  }

  spiTransfer(segmentNumber, OP_DISPLAYTEST, 0);
  // scanlimit is set to max on startup
  setScanLimit(segmentNumber, 7);
  // decode is done in source
  spiTransfer(segmentNumber, OP_DECODEMODE, 0);
  clearSegment(segmentNumber);
  // we go into shutdown-mode on startup
  activateSegment(segmentNumber);

  setIntensity(segmentNumber, this->conf.IntensityLevel);

  updateSegment(segmentNumber);
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::updateSegments() {
  if (!initilized) {
    return;
  }

  for (unsigned int seg = 0; seg < conf.SegmentCount(); seg++) {
    updateSegment(seg);
  }
}

template <size_t columns, size_t rows>
void LedController<columns,rows>::updateSegment(unsigned int segmentNumber) {
  if (!initilized) {
    return;
  }

  for (unsigned int row = 0; row < 8; row++) {
    spiTransfer(segmentNumber, row + 1, LedStates[segmentNumber][row]);
  }
}

template <size_t columns, size_t rows>
byte LedController<columns,rows>::reverse(byte var) {
  byte ret = 0x00;
  for (unsigned int i = 0; i < 8; i++) {
    if (var & (0x01U << i)) {
      ret |= 0x80U >> i;
    }
  }
  return ret;
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::makeColumns(ByteBlock rowArray) {
  auto columnArray = ByteBlock();

  for (unsigned int i = 0; i < 8; i++) {
    columnArray[i] = 0x00;
    for (unsigned int j = 0; j < 8; j++) {
      columnArray[i] |= (0x01 & (rowArray[j] >> (7 - i))) << (7 - j);
    }
  }

  return columnArray;
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::reverse(ByteBlock input) {
  auto reversedInput = ByteBlock();

  for (unsigned int i = 0; i < 8; i++) {
    reversedInput[i] = reverse(input[i]);
  }

  return reversedInput;
}

template <size_t columns, size_t rows>
ByteBlock LedController<columns,rows>::rotate180(ByteBlock input) {
  auto rotatedInput = ByteBlock();

  for (unsigned int i = 0; i < 8; i++) {
    rotatedInput[7 - i] = reverse(input[i]);
  }

  return rotatedInput;
}

template <size_t columns, size_t rows>
const controller_configuration<columns,rows>& LedController<columns,rows>::getConfig() { return conf; }

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::makeColumns(ByteBlock rowArray, ByteBlock *columnArray) {
  if (!initilized || columnArray == nullptr) {
    return;
  };

  for (unsigned int i = 0; i < 8; i++) {
    (*columnArray)[i] = 0x00;
    for (unsigned int j = 0; j < 8; j++) {
      (*columnArray)[i] |= (0x01 & (rowArray[j] >> (7 - i))) << (7 - j);
    }
  }
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::reverse(ByteBlock input, ByteBlock *reversedInput) {
  if (reversedInput == nullptr) {
    return;
  }

  for (unsigned int i = 0; i < 8; i++) {
    (*reversedInput)[i] = reverse(input[i]);
  }
}

// to be removed for version 2.2.0
template <size_t columns, size_t rows>
void LedController<columns,rows>::rotate180(ByteBlock input, ByteBlock *rotatedInput) {
  if (rotatedInput == nullptr) {
    return;
  }

  for (unsigned int i = 0; i < 8; i++) {
    (*rotatedInput)[7 - i] = reverse(input[i]);
  }
}