#include "LedController.hpp"

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

LedController::LedController(
    unsigned int dataPin, 
    unsigned int clkPin, 
    unsigned int csPin, 
    unsigned int numSegments
){
    SPI_DIN = dataPin;
    SPI_CLK = clkPin;
    SPI_CS = csPin;
    SegmentCount = numSegments;

    if(SegmentCount > MAX_SEGMENTS){
        SegmentCount = MAX_SEGMENTS;
    }

    for(unsigned int i = 0; i < spidata.size();i++){
        spidata.at(i) = 0x00;
    }

    for(auto x:LedStates){
        for(unsigned int i = 0; i < x.size();i++){
            x.at(i) = 0x00;
        }
    }

    for(unsigned int i = 0; i < MAX_SEGMENTS;i++){
        emptyRow.at(i)=0x00;
    }

    pinMode(SPI_DIN,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,HIGH);
    
    for(int i=0;i < SegmentCount;i++) {
        spiTransfer(i,OP_DISPLAYTEST,0);
        //scanlimit is set to max on startup
        setScanLimit(i,7);
        //decode is done in source
        spiTransfer(i,OP_DECODEMODE,0);
        clearSegment(i);
        //we go into shutdown-mode on startup
        activateSegment(i);

        setIntensity(i,1);
    }
}

void LedController::resetMatrix(){
    activateAllSegments();
    setIntensity(1);
    clearMatrix();
}


void LedController::clearMatrix(){
    for(unsigned int i = 0;i < SegmentCount;i++){
        clearSegment(i);   // Clear Segments
    }
}

void LedController::setIntensity(unsigned int newIntesityLevel){
    if(newIntesityLevel > 15){
        return;
    }

    //std::lock_guard<std::mutex> lock(mut_IntensityLevel);
    IntensityLevel = newIntesityLevel;

    for(unsigned int i=0;i < SegmentCount;i++){
        setIntensity(i,IntensityLevel);
    }
}

void LedController::setIntensity(unsigned int segmentNumber, unsigned int newIntesityLevel){
    if (newIntesityLevel > 15 || segmentNumber >= SegmentCount){
        return;
    }

    spiTransfer(segmentNumber, OP_INTENSITY,newIntesityLevel);
}

void LedController::displayOnSegment(unsigned int segmentindex, byte data[8]){
    if(segmentindex >= SegmentCount){
        return;
    }

    for(int i=0;i < 8;i++){
        setRow(segmentindex,i,data[i]);
    }
}

unsigned int LedController::getSegmentCount(){
    return SegmentCount;
}

void LedController::shutdownSegment(unsigned int segmentNumber){
    if(segmentNumber >= SegmentCount){
        return;
    }

    spiTransfer(segmentNumber, OP_SHUTDOWN,0);
}

void LedController::activateSegment(unsigned int segmentNumber){
    if(segmentNumber >= SegmentCount){
        return;
    }

    spiTransfer(segmentNumber, OP_SHUTDOWN,1);
}

void LedController::shutdownAllSegments(){
    for(unsigned int i = 0; i < SegmentCount;i++){
        shutdownSegment(i);
    }
}

void LedController::activateAllSegments(){
    for(unsigned int i = 0; i < SegmentCount;i++){
        activateSegment(i);
    }
}

void LedController::spiTransfer(unsigned int segment, byte opcode, byte data) {
    if(segment >= SegmentCount){
        return;
    }

    //Create an array with the data to shift out
    unsigned int offset = segment*2;
    unsigned int maxbytes = SegmentCount*2;

    for(int i=0;i < maxbytes;i++){
        spidata[i]=(byte)0;
    }

    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;

    //enable the line 
    digitalWrite(SPI_CS,LOW);

    //Now shift out the data 
    for(int i=maxbytes;i > 0;i--){
        shiftOut(SPI_DIN,SPI_CLK,MSBFIRST,spidata[i-1]);
    }

    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);
}   

void LedController::setScanLimit(unsigned int segmentNumber, unsigned int limit){
    if(segmentNumber >= SegmentCount){return;};
    if(limit < 8) {spiTransfer(segmentNumber, OP_SCANLIMIT, limit); };
}

void LedController::clearSegment(unsigned int segmentNumber){
    if(segmentNumber>=SegmentCount){
        return;
    }

    for(int i=0;i < 8;i++) {
        LedStates.at(segmentNumber).at(i) = 0x00;
        spiTransfer(segmentNumber, i+1, LedStates.at(segmentNumber).at(i));
    }
}

void LedController::setRow(unsigned int segmentNumber, unsigned int row, byte value){
    if(segmentNumber >= SegmentCount || row > 7){
        return;
    }
    
    LedStates.at(segmentNumber).at(row) = value;
    spiTransfer(segmentNumber, row+1, LedStates.at(segmentNumber).at(row));
}

 
 void LedController::setLed(unsigned int segmentNumber, unsigned int row, unsigned int column, boolean state) {
    if( row > 7 || column > 7 || segmentNumber >= SegmentCount){ return;};

    byte val=B10000000 >> column;

    if(state)
        LedStates.at(segmentNumber).at(row)=LedStates.at(segmentNumber).at(row)|val;
    else {
        val=~val;
        LedStates.at(segmentNumber).at(row)=LedStates.at(segmentNumber).at(row)&val;
    }
    spiTransfer(segmentNumber, row+1,LedStates.at(segmentNumber).at(row));
}


void LedController::setColumn(unsigned int segmentNumber, unsigned int col, byte value) {
    byte val;

    if( segmentNumber >= SegmentCount || col > 7){return;};
    
    for(int row=0;row<8;row++) {
        //val = value & (0x01 << row);
        val=value >> (7-row);
        val=val & 0x01;
        setLed(segmentNumber, row, col, val);
    }
}

void LedController::setDigit(unsigned int segmentNumber, unsigned int digit, byte value, boolean dp) {
    if(segmentNumber >= SegmentCount || digit > 7 || value > 15) {return;};

    byte v = pgm_read_byte_near(charTable + value); 
    if(dp) {v |= B10000000; };
    LedStates.at(segmentNumber).at(digit) = v;
    spiTransfer(segmentNumber, digit+1,v);
}

void LedController::setChar(unsigned int segmentNumber, unsigned int digit, char value, boolean dp) {
    if(segmentNumber >= SegmentCount || digit>7) {return;};
    
    byte index = (byte)value;
    if(index >127) {
        //no defined beyond index 127, so we use the space char
        index=32;
    }

    byte v = pgm_read_byte_near(charTable + index); 
    if(dp){ v |= B10000000; };

    LedStates.at(segmentNumber).at(digit) = v;
    spiTransfer(segmentNumber, digit+1, v);
}

void LedController::refreshSegments(){
    for(unsigned int seg = 0; seg < SegmentCount; seg++){
        for(unsigned int row = 0; row < 8; row++){
            spiTransfer(seg, row+1, LedStates.at(seg).at(row));
        }
    }
}

std::array<byte,8> LedController::makeColumns(std::array<byte,8> rowArray){
    std::array<byte,8> columnArray;

    for(unsigned int i = 0; i < 8;i++){
        columnArray.at(i) = 0x00;
        for(unsigned int j = 0; j < 8; j++){
            columnArray.at(i) |= (0x01 & ( rowArray.at(j) >> (7-i) )) << (7-j);
        }
    }

    return columnArray;
}

byte LedController::moveRight(byte shiftedInColumn){
    byte returnValue = 0x00;

    for(unsigned int i = 0; i < 8;i++){
        if(LedStates.at(SegmentCount-1).at(i) & 0x80){returnValue |= 0x01 << i; };
    }

    for(int seg = SegmentCount-1;seg >= 0;seg--){
        for(int row=0;row < 8;row++){
            LedStates.at(seg).at(row) = LedStates.at(seg).at(row)<<1;

            if(seg != 0 && LedStates.at(seg-1).at(row) & 0x80){ LedStates.at(seg).at(row)++; };

        }

    }

    setColumn(0,7,shiftedInColumn);

    refreshSegments();

    return returnValue;
}


byte LedController::moveLeft(byte shiftedInColumn){
    byte returnValue = 0x00;

    for(unsigned int i = 0; i < 8;i++){
        if(LedStates.at(SegmentCount-1).at(i) & 0x80){returnValue |= 0x01 << i; };
    }

    for(int seg = 0;seg < SegmentCount;seg++){
        for(int row=0;row < 8;row++){
            LedStates.at(seg).at(row) = LedStates.at(seg).at(row)>>1;

            if(seg != SegmentCount-1 && LedStates.at(seg+1).at(row) & 0x01){ LedStates.at(seg).at(row) |= 0x80; };

        }

    }

    setColumn(SegmentCount-1,0,shiftedInColumn);

    refreshSegments();

    return returnValue;
}

std::array<byte,MAX_SEGMENTS> LedController::moveDown(std::array<byte,MAX_SEGMENTS> shiftedInRow ){
    auto retVal = std::array<byte,MAX_SEGMENTS>(emptyRow);
    for(unsigned int i = 0; i < SegmentCount;i++){
        retVal.at(i) = LedStates.at(i).at(0);
    }

    for (unsigned int i = 0; i < 7; i++){
        for(unsigned int seg = 0; seg < SegmentCount; seg++){
            LedStates.at(seg).at(i) = LedStates.at(seg).at(i+1);
        }
        
    }

    for(unsigned int i = 0; i < SegmentCount;i++){
        LedStates.at(i).at(7) = shiftedInRow.at(i);
    }

    refreshSegments();

    return retVal;
    
}

std::array<byte,MAX_SEGMENTS> LedController::moveUp(std::array<byte,MAX_SEGMENTS> shiftedInRow ){
    auto retVal = std::array<byte,MAX_SEGMENTS>(emptyRow);
    for(unsigned int i = 0; i < SegmentCount;i++){
        retVal.at(i) = LedStates.at(i).at(7);
    }

    for (unsigned int i = 7; i > 0; i--){
        for(unsigned int seg = 0; seg < SegmentCount; seg++){
            LedStates.at(seg).at(i) = LedStates.at(seg).at(i-1);
        }
        
    }

    for(unsigned int i = 0; i < SegmentCount;i++){
        LedStates.at(i).at(0) = shiftedInRow.at(i);
    }

    refreshSegments();

    return retVal;
    
}

std::array<byte,MAX_SEGMENTS> LedController::moveUp(){
    auto inVal = std::array<byte,MAX_SEGMENTS>(emptyRow);
    return moveUp(inVal);
}

std::array<byte,MAX_SEGMENTS> LedController::moveDown(){
    auto inVal = std::array<byte,MAX_SEGMENTS>(emptyRow);
    return moveDown(inVal);
}

byte LedController::reverse(byte var){
    byte ret = 0x00;
    for(unsigned int i = 0; i < 8; i++){
        if(var & (0x01U<<i)){
            ret |= 0x80U>>i;
        }
        
    }
    return ret;
}

std::array<byte,8> LedController::reverse(std::array<byte,8> input){
    decltype(input) ret;

    for(unsigned int i = 0; i < input.size();i++){
        ret.at(i) = reverse(input.at(i));
    }

    return ret;
}