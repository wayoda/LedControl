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

LedController::LedController():LedController(27,25,26,4){};

LedController::LedController(
    unsigned int dataPin, 
    unsigned int clkPin, 
    unsigned int csPin, 
    unsigned int numDevices
){
    SPI_DIN = dataPin;
    SPI_CLK = clkPin;
    SPI_CS = csPin;
    SegmentCount = numDevices;

    if(SegmentCount > MAX_SEGMENTS){
        SegmentCount = MAX_SEGMENTS;
    }

    spidata.reserve(2 * SegmentCount);
    status.reserve(SegmentCount);
    for(auto x:status){
        x.reserve(8);
    }

    resetMatrix();
}

LedController::~LedController(){

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

    std::lock_guard<std::mutex> lock(mut_IntensityLevel);
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
        status.at(segmentNumber).at(i) = 0x00;
        spiTransfer(segmentNumber, i+1, status.at(segmentNumber).at(i));
    }
}

void LedController::setRow(unsigned int segmentNumber, unsigned int row, byte value){
    if(segmentNumber >= SegmentCount || row > 7){
        return;
    }
    
    status.at(segmentNumber).at(row) = value;
    spiTransfer(segmentNumber, row+1, status.at(segmentNumber).at(row));
}