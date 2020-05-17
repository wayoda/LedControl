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

LedController::~LedController(){
    initilized = false;
    delete[] LedStates;
    delete[] spidata;
    delete[] emptyRow;
}

LedController::LedController(){};

LedController::LedController(unsigned int csPin, unsigned int numSegments):LedController::LedController(MOSI,SCK,csPin,numSegments,true){};

LedController::LedController(
    unsigned int dataPin, 
    unsigned int clkPin, 
    unsigned int csPin, 
    unsigned int numSegments, 
    bool useHardwareSpiParam
){
    init(dataPin,clkPin,csPin,numSegments,useHardwareSpiParam); 
}

LedController::LedController(const LedController& other){
    if(!other.initilized){
        return;
    }

    init(other.SPI_MOSI, other.SPI_CLK, other.SPI_CS, other.SegmentCount, other.useHardwareSpi);

    for(unsigned int i = 0; i < SegmentCount;i++){
        for(unsigned int j = 0; i < 8;i++){
            LedStates[i][j] = other.LedStates[i][j];
        }
    }

    for(unsigned int i = 0; i < SegmentCount * 2;i++){
        spidata[i] = other.spidata[i];
    }

    //other.~LedController();

    refreshSegments();

}

void LedController::init(unsigned int csPin, unsigned int numSegments){
    return init(MOSI,SCK,csPin,numSegments,true);
}

void LedController::init(
    unsigned int dataPin, 
    unsigned int clkPin, 
    unsigned int csPin, 
    unsigned int numSegments, 
    bool useHardwareSpiParam
){
    if(initilized){
        return;
    }

    Serial.println("Beginning inilization");

    SPI_MOSI = dataPin;
    SPI_CLK = clkPin;
    SPI_CS = csPin;
    SegmentCount = numSegments;
    useHardwareSpi = useHardwareSpiParam;

    LedStates = new ByteBlock[SegmentCount];
    spidata = new byte[SegmentCount*2];
    emptyRow = new byte[SegmentCount];

    for(unsigned int i = 0; i < SegmentCount*2;i++){
        spidata[i] = 0x00;
    }

    for(unsigned int j = 0; j < SegmentCount;j++){
        for(unsigned int i = 0; i < 8;i++){
            LedStates[j][i] = 0x00;
        }
    }

    for(unsigned int i = 0; i < SegmentCount;i++){
        emptyRow[i]=0x00;
    }

    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);

    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,LOW);

    if(useHardwareSpi){
        SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
		SPI.begin();
    }

    digitalWrite(SPI_CS,HIGH);
    
    initilized = true;
    for(unsigned int i=0;i < SegmentCount;i++) {
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

    Serial.println("finished initilization");

}

bool LedController::isInitilized(){
    return initilized;
}

void LedController::resetMatrix(){
    if(!initilized){
        return;
    }

    activateAllSegments();
    setIntensity(1);
    clearMatrix();
}

void LedController::clearMatrix(){
    if(!initilized){
        return;
    }

    for(unsigned int i = 0;i < SegmentCount;i++){
        clearSegment(i);   // Clear Segments
    }
}

void LedController::setIntensity(unsigned int newIntesityLevel){
    if(newIntesityLevel > 15 || !initilized){
        return;
    }

    //std::lock_guard<std::mutex> lock(mut_IntensityLevel);
    IntensityLevel = newIntesityLevel;

    for(unsigned int i=0;i < SegmentCount;i++){
        setIntensity(i,IntensityLevel);
    }
}

void LedController::setIntensity(unsigned int segmentNumber, unsigned int newIntesityLevel){
    if (newIntesityLevel > 15 || !initilized || segmentNumber >= SegmentCount){
        return;
    }

    spiTransfer(segmentNumber, OP_INTENSITY,newIntesityLevel);
}

void LedController::displayOnSegment(unsigned int segmentindex, ByteBlock data){
    if(!initilized || segmentindex >= SegmentCount){
        return;
    }

    for(int i=0;i < 8;i++){
        setRow(segmentindex,i,data[i]);
    }
}

void LedController::getSegmentData(unsigned int segmentindex, ByteBlock* resultLocation){
    if(!initilized || segmentindex >= SegmentCount || resultLocation == nullptr){
        return;
    }

    for(unsigned int i = 0; i < 8;i++){
        (*resultLocation)[i] = LedStates[segmentindex][i];
    }
}

unsigned int LedController::getSegmentCount(){
    if(!initilized){
        return 0;
    }

    return SegmentCount;
}

void LedController::shutdownSegment(unsigned int segmentNumber){
    if(!initilized || segmentNumber >= SegmentCount){
        return;
    }

    spiTransfer(segmentNumber, OP_SHUTDOWN,0);
}

void LedController::activateSegment(unsigned int segmentNumber){
    if(!initilized || segmentNumber >= SegmentCount){
        return;
    }

    spiTransfer(segmentNumber, OP_SHUTDOWN,1);
}

void LedController::shutdownAllSegments(){
    if(!initilized){
        return;
    }

    for(unsigned int i = 0; i < SegmentCount;i++){
        shutdownSegment(i);
    }
}

void LedController::activateAllSegments(){
    if(!initilized){
        return;
    }

    for(unsigned int i = 0; i < SegmentCount;i++){
        activateSegment(i);
    }
}

void LedController::spiTransfer(unsigned int segment, byte opcode, byte data) {
    if(!initilized || segment >= SegmentCount){
        return;
    }

    //Create an array with the data to shift out
    unsigned int offset = segment*2;
    unsigned int maxbytes = SegmentCount*2;

    for(unsigned int i=0;i < maxbytes;i++){
        spidata[i]=0x00;
    }

    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;

    //enable the line 
    digitalWrite(SPI_CS,LOW);

    if(useHardwareSpi){
        SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    }

    //Now shift out the data 
    for(int i=maxbytes;i > 0;i--){
        if(useHardwareSpi){
            SPI.transfer(spidata[i-1]);
        }else{
            shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
        }
    }

    if(useHardwareSpi){
        SPI.endTransaction();
    }

    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);

}   

void LedController::setScanLimit(unsigned int segmentNumber, unsigned int limit){
    if(!initilized || segmentNumber >= SegmentCount){return;};
    if(limit < 8) {spiTransfer(segmentNumber, OP_SCANLIMIT, limit); };
}

void LedController::clearSegment(unsigned int segmentNumber){
    if(!initilized || segmentNumber>=SegmentCount){
        return;
    }

    for(int i=0;i < 8;i++) {
        LedStates[segmentNumber][i] = 0x00;
        spiTransfer(segmentNumber, i+1, LedStates[segmentNumber][i]);
    }
}

void LedController::setRow(unsigned int segmentNumber, unsigned int row, byte value){
    if(!initilized || segmentNumber >= SegmentCount || row > 7 || LedStates[segmentNumber][row] == value){
        return;
    }
    
    LedStates[segmentNumber][row] = value;
    spiTransfer(segmentNumber, row+1, LedStates[segmentNumber][row]);
}

byte LedController::getRow(unsigned int segmentNumber, unsigned int row){
    if(!initilized || segmentNumber >= SegmentCount || row > 7){
        return 0x00;
    }
    
    return LedStates[segmentNumber][row];
}

 
void LedController::setLed(unsigned int segmentNumber, unsigned int row, unsigned int column, boolean state) {
    if(!initilized ||  row > 7 || column > 7 || segmentNumber >= SegmentCount){ return;};

    byte val=B10000000 >> column;

    if(state)
        LedStates[segmentNumber][row]=LedStates[segmentNumber][row]|val;
    else {
        val=~val;
        LedStates[segmentNumber][row]=LedStates[segmentNumber][row]&val;
    }
    spiTransfer(segmentNumber, row+1,LedStates[segmentNumber][row]);
}


void LedController::setColumn(unsigned int segmentNumber, unsigned int col, byte value) {
    if(!initilized || segmentNumber >= SegmentCount || col > 7){return;};
    
    byte val;

    for(int row=0;row<8;row++) {
        //val = value & (0x01 << row);
        val=value >> (7-row);
        val=val & 0x01;
        setLed(segmentNumber, row, col, val);
    }
}

void LedController::setDigit(unsigned int segmentNumber, unsigned int digit, byte value, boolean dp) {
    if(!initilized || segmentNumber >= SegmentCount || digit > 7 || value > 15) {return;};

    byte v = pgm_read_byte_near(charTable + value); 
    if(dp) {v |= B10000000; };
    LedStates[segmentNumber][digit] = v;
    spiTransfer(segmentNumber, digit+1,v);
}

void LedController::setChar(unsigned int segmentNumber, unsigned int digit, char value, boolean dp) {
    if(!initilized || segmentNumber >= SegmentCount || digit>7) {return;};
    
    byte index = (byte)value;
    if(index >127) {
        //no defined beyond index 127, so we use the space char
        index=32;
    }

    byte v = pgm_read_byte_near(charTable + index); 
    if(dp){ v |= B10000000; };

    LedStates[segmentNumber][digit] = v;
    spiTransfer(segmentNumber, digit+1, v);
}

void LedController::refreshSegments(){
    if(!initilized){
        return;
    }

    for(unsigned int seg = 0; seg < SegmentCount; seg++){
        for(unsigned int row = 0; row < 8; row++){
            spiTransfer(seg, row+1, LedStates[seg][row]);
        }
    }
}

byte LedController::moveRight(byte shiftedInColumn){
    if(!initilized){
        return 0x00;
    }

    byte returnValue = 0x00;

    for(unsigned int i = 0; i < 8;i++){
        if(LedStates[SegmentCount-1][i] & 0x80){returnValue |= 0x01 << i; };
    }

    for(int seg = SegmentCount-1;seg >= 0;seg--){
        for(int row=0;row < 8;row++){
            LedStates[seg][row] = LedStates[seg][row]<<1;

            if(seg != 0 && LedStates[seg-1][row] & 0x80){ LedStates[seg][row]++; };

        }

    }

    setColumn(0,7,shiftedInColumn);

    refreshSegments();

    return returnValue;
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

byte LedController::moveLeft(byte shiftedInColumn){
    if(!initilized){
        return 0x00;
    }

    byte returnValue = 0x00;

    for(unsigned int i = 0; i < 8;i++){
        if(LedStates[SegmentCount-1][i] & 0x80){returnValue |= 0x01 << i; };
    }

    for(unsigned int seg = 0;seg < SegmentCount;seg++){
        for(unsigned int row=0;row < 8;row++){
            LedStates[seg][row] = LedStates[seg][row]>>1;

            if(seg != SegmentCount-1 && LedStates[seg+1][row] & 0x01){ LedStates[seg][row] |= 0x80; };

        }

    }

    setColumn(SegmentCount-1,0,shiftedInColumn);

    refreshSegments();

    return returnValue;
}

//The plain C array functions

void LedController::createEmptyRow(byte** row){
    if(!initilized || row == nullptr || *row == nullptr){return;};

    for(unsigned int i = 0; i < SegmentCount;i++){
        (*row)[i] = 0x00;
    }
}

void LedController::makeColumns(ByteBlock rowArray, ByteBlock* columnArray){
    if(!initilized || columnArray == nullptr){return; };

    for(unsigned int i = 0; i < 8;i++){
        (*columnArray)[i] = 0x00;
        for(unsigned int j = 0; j < 8; j++){
            (*columnArray)[i] |= (0x01 & ( rowArray[j] >> (7-i) )) << (7-j);
        }
    }

}

void LedController::moveDown(byte* shiftedInRow, byte** shiftedOutRow){
    if(!initilized){
        return;
    }

    createEmptyRow(shiftedOutRow);
    if(shiftedOutRow != nullptr && *shiftedOutRow != nullptr){
        for(unsigned int i = 0; i < SegmentCount;i++){
            (*shiftedOutRow)[i] = LedStates[i][0];
        }
    }

    for (unsigned int i = 0; i < 7; i++){
        for(unsigned int seg = 0; seg < SegmentCount; seg++){
            LedStates[seg][i] = LedStates[seg][i+1];
        }
        
    }

    for(unsigned int i = 0; i < SegmentCount;i++){
        LedStates[i][7] = shiftedInRow[i];
    }

    refreshSegments();
    
}

void LedController::moveUp(byte* shiftedInRow, byte** shiftedOutRow){
    if(!initilized){
        return;
    }

    createEmptyRow(shiftedOutRow);
    if(shiftedOutRow != nullptr && *shiftedOutRow != nullptr){
        for(unsigned int i = 0; i < SegmentCount;i++){
            (*shiftedOutRow)[i] = LedStates[i][7];
        }
    }

    for (unsigned int i = 7; i > 0; i--){
        for(unsigned int seg = 0; seg < SegmentCount; seg++){
            LedStates[seg][i] = LedStates[seg][i-1];
        }
        
    }

    for(unsigned int i = 0; i < SegmentCount;i++){
        LedStates[i][0] = shiftedInRow[i];
    }

    refreshSegments();
    
}

void LedController::moveUp(byte** shiftedOutRow){
    if(!initilized){
        return;
    }

    byte* inVal = new byte[SegmentCount];
    createEmptyRow(&inVal);
    moveUp(inVal,shiftedOutRow);
    delete[] inVal;
}

void LedController::moveDown(byte** shiftedOutRow){
    if(!initilized){
        return;
    }

    byte* inVal = new byte[SegmentCount];
    createEmptyRow(&inVal);
    moveDown(inVal,shiftedOutRow);
    delete[] inVal;
}

void LedController::moveDown(){
    if(!initilized){
        return;
    }

    byte* inVal = new byte[SegmentCount];
    createEmptyRow(&inVal);
    moveDown(inVal,nullptr);
    delete[] inVal;
}

void LedController::moveUp(){
    if(!initilized){
        return;
    }

    byte* inVal = new byte[SegmentCount];
    createEmptyRow(&inVal);
    moveUp(inVal,nullptr);
    delete[] inVal;

}


void LedController::reverse(ByteBlock input, ByteBlock* reversedInput){
    if(reversedInput == nullptr){
        return;
    }

    for(unsigned int i = 0; i < 8;i++){
        (*reversedInput)[i] = reverse(input[i]);
    }
}

void LedController::rotate180(ByteBlock input, ByteBlock* rotatedInput){
    if(rotatedInput == nullptr){
        return;
    }

    for(unsigned int i = 0; i < 8;i++){
        (*rotatedInput)[7 - i] = reverse(input[i]);
    }
}
