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

LedController::LedController(unsigned int csPin, unsigned int numSegments):LedController::LedController(MOSI,SCK,csPin,numSegments,true){};

LedController::LedController(
    unsigned int dataPin, 
    unsigned int clkPin, 
    unsigned int csPin, 
    unsigned int numSegments, 
    bool useHardwareSpiParam
){
    SPI_MOSI = dataPin;
    SPI_CLK = clkPin;
    SPI_CS = csPin;
    SegmentCount = numSegments;
    useHardwareSpi = useHardwareSpiParam;

    if(SegmentCount > MAX_SEGMENTS){
        SegmentCount = MAX_SEGMENTS;
    }

    for(unsigned int i = 0; i < MAX_SEGMENTS*2;i++){
        spidata[i] = 0x00;
    }

    for(auto x:LedStates){
        for(unsigned int i = 0; i < 8;i++){
            x[i] = 0x00;
        }
    }

    for(unsigned int i = 0; i < MAX_SEGMENTS;i++){
        emptyRow[i]=0x00;
    }

    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);

    pinMode(SPI_CS,OUTPUT);

    if(useHardwareSpi){
        SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
		SPI.begin();
    }

    digitalWrite(SPI_CS,HIGH);
    
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

void LedController::displayOnSegment(unsigned int segmentindex, ByteBlock data){
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

    for(unsigned int i=0;i < maxbytes;i++){
        spidata[i]=(byte)0;
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
    if(segmentNumber >= SegmentCount){return;};
    if(limit < 8) {spiTransfer(segmentNumber, OP_SCANLIMIT, limit); };
}

void LedController::clearSegment(unsigned int segmentNumber){
    if(segmentNumber>=SegmentCount){
        return;
    }

    for(int i=0;i < 8;i++) {
        LedStates[segmentNumber][i] = 0x00;
        spiTransfer(segmentNumber, i+1, LedStates[segmentNumber][i]);
    }
}

void LedController::setRow(unsigned int segmentNumber, unsigned int row, byte value){
    if(segmentNumber >= SegmentCount || row > 7){
        return;
    }
    
    LedStates[segmentNumber][row] = value;
    spiTransfer(segmentNumber, row+1, LedStates[segmentNumber][row]);
}

 
 void LedController::setLed(unsigned int segmentNumber, unsigned int row, unsigned int column, boolean state) {
    if( row > 7 || column > 7 || segmentNumber >= SegmentCount){ return;};

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
    LedStates[segmentNumber][digit] = v;
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

    LedStates[segmentNumber][digit] = v;
    spiTransfer(segmentNumber, digit+1, v);
}

void LedController::refreshSegments(){
    for(unsigned int seg = 0; seg < SegmentCount; seg++){
        for(unsigned int row = 0; row < 8; row++){
            spiTransfer(seg, row+1, LedStates[seg][row]);
        }
    }
}

byte LedController::moveRight(byte shiftedInColumn){
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

#if (STD_CAPABLE > 0)

    ByteBlock LedController::makeColumns(ByteBlock rowArray){
        ByteBlock columnArray;

        for(unsigned int i = 0; i < 8;i++){
            columnArray[i] = 0x00;
            for(unsigned int j = 0; j < 8; j++){
                columnArray[i] |= (0x01 & ( rowArray[j] >> (7-i) )) << (7-j);
            }
        }

        return columnArray;
    }

    ByteRow LedController::moveDown(ByteRow shiftedInRow ){
        auto retVal = ByteRow(emptyRow);
        for(unsigned int i = 0; i < SegmentCount;i++){
            retVal[i] = LedStates[i][0];
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

        return retVal;
        
    }

    ByteRow LedController::moveUp(ByteRow shiftedInRow ){
        auto retVal = ByteRow(emptyRow);
        for(unsigned int i = 0; i < SegmentCount;i++){
            retVal[i] = LedStates[i][7];
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

        return retVal;
        
    }

    ByteRow LedController::moveUp(){
        auto inVal = ByteRow(emptyRow);
        return moveUp(inVal);
    }

    ByteRow LedController::moveDown(){
        auto inVal = ByteRow(emptyRow);
        return moveDown(inVal);
    }


    ByteBlock LedController::reverse(ByteBlock input){
        decltype(input) ret;

        for(unsigned int i = 0; i < input.size();i++){
            ret[i] = reverse(input[i]);
        }

        return ret;
    }

    ByteBlock LedController::rotate180(ByteBlock input){
        decltype(input) ret;

        for(unsigned int i = 0; i < input.size();i++){
            ret[input.size()-(i+1)] = reverse(input[i]);
        }

        return ret;
    }

#else

    int LedController::createEmptyRow(C_ByteRow* row){
        if(row == nullptr){return 0;};

        for(unsigned int i = 0; i < MAX_SEGMENTS;i++){
            (*row)[i] = 0x00;
        }

        return 1;
    }

    void LedController::makeColumns(C_ByteBlock rowArray, C_ByteBlock* columnArray){
        if(columnArray == nullptr){return; };

        for(unsigned int i = 0; i < 8;i++){
            (*columnArray)[i] = 0x00;
            for(unsigned int j = 0; j < 8; j++){
                (*columnArray)[i] |= (0x01 & ( rowArray[j] >> (7-i) )) << (7-j);
            }
        }

    }

    void LedController::moveDown(C_ByteRow shiftedInRow, C_ByteRow* shiftedOutRow){
        if(createEmptyRow(shiftedOutRow) != 0){
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

    void LedController::moveUp(C_ByteRow shiftedInRow, C_ByteRow* shiftedOutRow){
        if(createEmptyRow(shiftedOutRow) != 0){
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

    void LedController::moveUp(C_ByteRow* shiftedOutRow){
        ByteRow inVal;
        createEmptyRow(&inVal);
        moveUp(inVal,shiftedOutRow);
    }

    void LedController::moveDown(C_ByteRow* shiftedOutRow){
        ByteRow inVal;
        createEmptyRow(&inVal);
        moveDown(inVal,shiftedOutRow);
    }

    void LedController::moveDown(){
        ByteRow inVal;
        createEmptyRow(&inVal);
        moveDown(inVal,nullptr);
    }

    void LedController::moveUp(){
        ByteRow inVal;
        createEmptyRow(&inVal);
        moveUp(inVal,nullptr);
    }


    void LedController::reverse(C_ByteBlock input, C_ByteBlock* reversedInput){
        if(reversedInput == nullptr){
            return;
        }

        for(unsigned int i = 0; i < 8;i++){
            (*reversedInput)[i] = reverse(input[i]);
        }
    }

    void LedController::rotate180(C_ByteBlock input, C_ByteBlock* rotatedInput){
        if(rotatedInput == nullptr){
            return;
        }

        for(unsigned int i = 0; i < 8;i++){
            (*rotatedInput)[7 - i] = reverse(input[i]);
        }
    }

#endif