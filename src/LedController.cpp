#include "LedController.hpp"

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

LedController::~LedController() {
  initilized = false;
  delete[] LedStates;
  delete[] spidata;
  delete[] emptyRow;
}

LedController::LedController(){};

LedController::LedController(unsigned int csPin, unsigned int numSegments) {
  init(csPin, numSegments);
};

LedController::LedController(unsigned int dataPin, unsigned int clkPin,
                             unsigned int csPin, unsigned int numSegments,
                             bool useHardwareSpiParam) {
  init(dataPin, clkPin, csPin, numSegments, useHardwareSpiParam);
}

LedController::LedController(const controller_configuration &config) {
  init(config);
};

LedController::LedController(const LedController &other) {
  if (!other.initilized) {
    return;
  }

  init(other.conf);

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    for (unsigned int j = 0; i < 8; i++) {
      LedStates[i][j] = other.LedStates[i][j];
    }
  }

  for (unsigned int i = 0; i < conf.SegmentCount * 2; i++) {
    spidata[i] = other.spidata[i];
  }

  refreshSegments();
}

void LedController::init(unsigned int csPin, unsigned int numSegments) {
  controller_configuration config;

  config.SegmentCount = numSegments;
  config.SPI_CS = csPin;
  config.useHardwareSpi = true;

  return init(config);
}

void LedController::init(unsigned int dataPin, unsigned int clkPin,
                         unsigned int csPin, unsigned int numSegments,
                         bool useHardwareSpiParam) {
  if (initilized) {
    return;
  }

  controller_configuration config;

  config.SPI_MOSI = dataPin;
  config.SPI_CLK = clkPin;
  config.SPI_CS = csPin;
  config.SegmentCount = numSegments;
  config.useHardwareSpi = useHardwareSpiParam;

  init(config);
}

void LedController::init(const controller_configuration &configuration) {
  if (initilized) {
    return;
  }

  if (!controller_configuration::isValidConfig(configuration)) {
    return;
  }

  Serial.println("Beginning inilization");

  conf = configuration;

  if (conf.useHardwareSpi) {
    conf.SPI_CLK = SCK;
    conf.SPI_MOSI = MOSI;
  }

  LedStates = new ByteBlock[conf.SegmentCount];
  spidata = new byte[conf.SegmentCount * 2];
  emptyRow = new byte[conf.SegmentCount];

  for (unsigned int i = 0; i < conf.SegmentCount * 2; i++) {
    spidata[i] = 0x00;
  }

  for (unsigned int j = 0; j < conf.SegmentCount; j++) {
    for (unsigned int i = 0; i < 8; i++) {
      LedStates[j][i] = 0x00;
    }
  }

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    emptyRow[i] = 0x00;
  }

  pinMode(conf.SPI_MOSI, OUTPUT);
  pinMode(conf.SPI_CLK, OUTPUT);

  pinMode(conf.SPI_CS, OUTPUT);
  digitalWrite(conf.SPI_CS, LOW);

  /*
  if(conf.row_SPI_CS != nullptr){
      for(unsigned int i = 0; i < conf.rows;i++){
          pinMode(conf.row_SPI_CS[i],OUTPUT);
          digitalWrite(conf.row_SPI_CS[i],LOW);
      }
  }
  */

  if (conf.useHardwareSpi) {
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.begin();
  }

  digitalWrite(conf.SPI_CS, HIGH);

  initilized = true;
  refreshSegments();

  Serial.println("finished initilization");
}

bool LedController::isInitilized() { return initilized; }

void LedController::resetMatrix() {
  if (!initilized) {
    return;
  }

  activateAllSegments();
  setIntensity(1);
  clearMatrix();
}

void LedController::clearMatrix() {
  if (!initilized) {
    return;
  }

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    clearSegment(i); // Clear Segments
  }
}

void LedController::setIntensity(unsigned int newIntesityLevel) {
  if (newIntesityLevel > 15 || !initilized) {
    return;
  }

  // std::lock_guard<std::mutex> lock(mut_IntensityLevel);
  conf.IntensityLevel = newIntesityLevel;

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    setIntensity(i, conf.IntensityLevel);
  }
}

void LedController::setIntensity(unsigned int segmentNumber,
                                 unsigned int newIntesityLevel) {
  if (newIntesityLevel > 15 || !initilized ||
      segmentNumber >= conf.SegmentCount) {
    return;
  }

  spiTransfer(segmentNumber, OP_INTENSITY, newIntesityLevel);
}

void LedController::displayOnSegment(unsigned int segmentindex,
                                     ByteBlock data) {
  if (!initilized || segmentindex >= conf.SegmentCount) {
    return;
  }

  for (int i = 0; i < 8; i++) {
    setRow(segmentindex, i, data[i]);
  }
}

void LedController::getSegmentData(unsigned int segmentindex,
                                   ByteBlock *resultLocation) {
  if (!initilized || segmentindex >= conf.SegmentCount ||
      resultLocation == nullptr) {
    return;
  }

  for (unsigned int i = 0; i < 8; i++) {
    (*resultLocation)[i] = LedStates[segmentindex][i];
  }
}

unsigned int LedController::getSegmentCount() {
  if (!initilized) {
    return 0;
  }

  return conf.SegmentCount;
}

void LedController::shutdownSegment(unsigned int segmentNumber) {
  if (!initilized || segmentNumber >= conf.SegmentCount) {
    return;
  }

  spiTransfer(segmentNumber, OP_SHUTDOWN, 0);
}

void LedController::activateSegment(unsigned int segmentNumber) {
  if (!initilized || segmentNumber >= conf.SegmentCount) {
    return;
  }

  spiTransfer(segmentNumber, OP_SHUTDOWN, 1);
}

void LedController::shutdownAllSegments() {
  if (!initilized) {
    return;
  }

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    shutdownSegment(i);
  }
}

void LedController::activateAllSegments() {
  if (!initilized) {
    return;
  }

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    activateSegment(i);
  }
}

void LedController::spiTransfer(unsigned int segment, byte opcode, byte data) {
  if (!initilized || segment >= conf.SegmentCount) {
    return;
  }

  // Create an array with the data to shift out
  unsigned int offset = segment * 2;
  unsigned int maxbytes = conf.SegmentCount * 2;

  for (unsigned int i = 0; i < maxbytes; i++) {
    spidata[i] = 0x00;
  }

  // put our device data into the array
  spidata[offset + 1] = opcode;
  spidata[offset] = data;

  // enable the line
  digitalWrite(conf.SPI_CS, LOW);

  if (conf.useHardwareSpi) {
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  }

  // Now shift out the data
  for (int i = maxbytes; i > 0; i--) {
    if (conf.useHardwareSpi) {
      SPI.transfer(spidata[i - 1]);
    } else {
      shiftOut(conf.SPI_MOSI, conf.SPI_CLK, MSBFIRST, spidata[i - 1]);
    }
  }

  if (conf.useHardwareSpi) {
    SPI.endTransaction();
  }

  // latch the data onto the display
  digitalWrite(conf.SPI_CS, HIGH);
}

void LedController::setScanLimit(unsigned int segmentNumber,
                                 unsigned int limit) {
  if (!initilized || segmentNumber >= conf.SegmentCount) {
    return;
  };
  if (limit < 8) {
    spiTransfer(segmentNumber, OP_SCANLIMIT, limit);
  };
}

void LedController::clearSegment(unsigned int segmentNumber) {
  if (!initilized || segmentNumber >= conf.SegmentCount) {
    return;
  }

  for (int i = 0; i < 8; i++) {
    LedStates[segmentNumber][i] = 0x00;
    spiTransfer(segmentNumber, i + 1, LedStates[segmentNumber][i]);
  }
}

void LedController::setRow(unsigned int segmentNumber, unsigned int row,
                           byte value) {
  if (!initilized || segmentNumber >= conf.SegmentCount || row > 7 ||
      LedStates[segmentNumber][row] == value) {
    return;
  }

  LedStates[segmentNumber][row] = value;
  spiTransfer(segmentNumber, row + 1, LedStates[segmentNumber][row]);
}

byte LedController::getRow(unsigned int segmentNumber, unsigned int row) {
  if (!initilized || segmentNumber >= conf.SegmentCount || row > 7) {
    return 0x00;
  }

  return LedStates[segmentNumber][row];
}

void LedController::setLed(unsigned int segmentNumber, unsigned int row,
                           unsigned int column, boolean state) {
  if (!initilized || row > 7 || column > 7 ||
      segmentNumber >= conf.SegmentCount) {
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

void LedController::setColumn(unsigned int segmentNumber, unsigned int col,
                              byte value) {
  if (!initilized || segmentNumber >= conf.SegmentCount || col > 7) {
    return;
  };

  byte val;

  for (int row = 0; row < 8; row++) {
    // val = value & (0x01 << row);
    val = value >> (7 - row);
    val = val & 0x01;
    setLed(segmentNumber, row, col, val);
  }
}

void LedController::setDigit(unsigned int segmentNumber, unsigned int digit,
                             byte value, boolean dp) {
  if (!initilized || segmentNumber >= conf.SegmentCount || digit > 7 ||
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

void LedController::setChar(unsigned int segmentNumber, unsigned int digit,
                            char value, boolean dp) {
  if (!initilized || segmentNumber >= conf.SegmentCount || digit > 7) {
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

void LedController::refreshSegments() {
  if (!initilized) {
    return;
  }

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    refreshSegment(i);
  }
}

void LedController::refreshSegment(unsigned int segmentNumber) {
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

void LedController::updateSegments() {
  if (!initilized) {
    return;
  }

  for (unsigned int seg = 0; seg < conf.SegmentCount; seg++) {
    updateSegment(seg);
  }
}

void LedController::updateSegment(unsigned int segmentNumber) {
  if (!initilized) {
    return;
  }

  for (unsigned int row = 0; row < 8; row++) {
    spiTransfer(segmentNumber, row + 1, LedStates[segmentNumber][row]);
  }
}

byte LedController::moveRight(byte shiftedInColumn) {
  if (!initilized) {
    return 0x00;
  }

  byte returnValue = 0x00;

  for (unsigned int i = 0; i < 8; i++) {
    if (LedStates[conf.SegmentCount - 1][i] & 0x80) {
      returnValue |= 0x01 << i;
    };
  }

  for (int seg = conf.SegmentCount - 1; seg >= 0; seg--) {
    for (int row = 0; row < 8; row++) {
      LedStates[seg][row] = LedStates[seg][row] << 1;

      if (seg != 0 && LedStates[seg - 1][row] & 0x80) {
        LedStates[seg][row]++;
      };
    }
  }

  setColumn(0, 7, shiftedInColumn);

  updateSegments();

  return returnValue;
}

byte LedController::reverse(byte var) {
  byte ret = 0x00;
  for (unsigned int i = 0; i < 8; i++) {
    if (var & (0x01U << i)) {
      ret |= 0x80U >> i;
    }
  }
  return ret;
}

byte LedController::moveLeft(byte shiftedInColumn) {
  if (!initilized) {
    return 0x00;
  }

  byte returnValue = 0x00;

  for (unsigned int i = 0; i < 8; i++) {
    if (LedStates[conf.SegmentCount - 1][i] & 0x80) {
      returnValue |= 0x01 << i;
    };
  }

  for (unsigned int seg = 0; seg < conf.SegmentCount; seg++) {
    for (unsigned int row = 0; row < 8; row++) {
      LedStates[seg][row] = LedStates[seg][row] >> 1;

      if (seg != conf.SegmentCount - 1 && LedStates[seg + 1][row] & 0x01) {
        LedStates[seg][row] |= 0x80;
      };
    }
  }

  setColumn(conf.SegmentCount - 1, 0, shiftedInColumn);

  updateSegments();

  return returnValue;
}

// The plain C array functions

void LedController::createEmptyRow(byte **row) {
  if (!initilized || row == nullptr || *row == nullptr) {
    return;
  };

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    (*row)[i] = 0x00;
  }
}

void LedController::makeColumns(ByteBlock rowArray, ByteBlock *columnArray) {
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

void LedController::moveDown(byte *shiftedInRow, byte **shiftedOutRow) {
  if (!initilized) {
    return;
  }

  createEmptyRow(shiftedOutRow);
  if (shiftedOutRow != nullptr && *shiftedOutRow != nullptr) {
    for (unsigned int i = 0; i < conf.SegmentCount; i++) {
      (*shiftedOutRow)[i] = LedStates[i][0];
    }
  }

  for (unsigned int i = 0; i < 7; i++) {
    for (unsigned int seg = 0; seg < conf.SegmentCount; seg++) {
      LedStates[seg][i] = LedStates[seg][i + 1];
    }
  }

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    LedStates[i][7] = shiftedInRow[i];
  }

  updateSegments();
}

void LedController::moveUp(byte *shiftedInRow, byte **shiftedOutRow) {
  if (!initilized) {
    return;
  }

  createEmptyRow(shiftedOutRow);
  if (shiftedOutRow != nullptr && *shiftedOutRow != nullptr) {
    for (unsigned int i = 0; i < conf.SegmentCount; i++) {
      (*shiftedOutRow)[i] = LedStates[i][7];
    }
  }

  for (unsigned int i = 7; i > 0; i--) {
    for (unsigned int seg = 0; seg < conf.SegmentCount; seg++) {
      LedStates[seg][i] = LedStates[seg][i - 1];
    }
  }

  for (unsigned int i = 0; i < conf.SegmentCount; i++) {
    LedStates[i][0] = shiftedInRow[i];
  }

  updateSegments();
}

void LedController::moveUp(byte **shiftedOutRow) {
  if (!initilized) {
    return;
  }

  byte *inVal = new byte[conf.SegmentCount];
  createEmptyRow(&inVal);
  moveUp(inVal, shiftedOutRow);
  delete[] inVal;
}

void LedController::moveDown(byte **shiftedOutRow) {
  if (!initilized) {
    return;
  }

  byte *inVal = new byte[conf.SegmentCount];
  createEmptyRow(&inVal);
  moveDown(inVal, shiftedOutRow);
  delete[] inVal;
}

void LedController::moveDown() {
  if (!initilized) {
    return;
  }

  byte *inVal = new byte[conf.SegmentCount];
  createEmptyRow(&inVal);
  moveDown(inVal, nullptr);
  delete[] inVal;
}

void LedController::moveUp() {
  if (!initilized) {
    return;
  }

  byte *inVal = new byte[conf.SegmentCount];
  createEmptyRow(&inVal);
  moveUp(inVal, nullptr);
  delete[] inVal;
}

void LedController::reverse(ByteBlock input, ByteBlock *reversedInput) {
  if (reversedInput == nullptr) {
    return;
  }

  for (unsigned int i = 0; i < 8; i++) {
    (*reversedInput)[i] = reverse(input[i]);
  }
}

void LedController::rotate180(ByteBlock input, ByteBlock *rotatedInput) {
  if (rotatedInput == nullptr) {
    return;
  }

  for (unsigned int i = 0; i < 8; i++) {
    (*rotatedInput)[7 - i] = reverse(input[i]);
  }
}

controller_configuration LedController::getConfig() { return conf; }
