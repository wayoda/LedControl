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
    Data = dataPin;
    Clock = clkPin;
    ChipSelect = csPin;
    SegmentCount = numDevices;

    lc = std::shared_ptr<LedControl>(new LedControl(Data,Clock,ChipSelect,SegmentCount));

    resetMatrix();
}

LedController::~LedController(){
    lc.reset();
}

void LedController::resetMatrix(){
    activateAllSegments();
    setIntensity(1);
    clearMatrix();
}


void LedController::clearMatrix(){
    for(int i = 0;i < SegmentCount;i++){
        lc->clearDisplay(i);     // Clear Displays
    }
}

void LedController::setIntensity(unsigned int newIntesityLevel){
    if(newIntesityLevel > 15){
        return;
    }

    std::lock_guard<std::mutex> lock(mut_IntensityLevel);
    IntensityLevel = newIntesityLevel;

    for(unsigned int i=0;i < SegmentCount;i++){
        lc->setIntensity(i,IntensityLevel);
    }
}

void LedController::displayOnSegment(unsigned int segmentindex, byte data[8]){
    if(segmentindex >= SegmentCount){
        return;
    }

    for(int i=0;i < 8;i++){
        lc->setRow(segmentindex,i,data[i]);
    }
}

std::shared_ptr<LedControl> getLedControl(){
    return std::shared_ptr<LedControl>();
}

unsigned int LedController::getSegmentCount(){
    return SegmentCount;
}

void LedController::shutdownSegment(unsigned int segmentNumber){
    if(segmentNumber >= SegmentCount){
        return;
    }

    lc->shutdown(segmentNumber,true);
}

void LedController::activateSegment(unsigned int segmentNumber){
    if(segmentNumber >= SegmentCount){
        return;
    }

    lc->shutdown(segmentNumber,false);
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

void LedController::spiTransfer(int addr, volatile byte opcode, volatile byte data) {
    //Create an array with the data to shift out
    int offset=addr*2;
    int maxbytes=maxDevices*2;

    for(int i=0;i<maxbytes;i++)
        spidata[i]=(byte)0;
    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;
    //enable the line 
    digitalWrite(SPI_CS,LOW);
    //Now shift out the data 
    for(int i=maxbytes;i>0;i--)
        shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);
}   
