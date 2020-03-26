#include "LedController.hpp"

#define CS 15

#define Segments 4

#define delayTime 200 // Delay between Frames

LedController lc = LedController(CS,Segments);  // Pins: DIN,CLK,CS, # of Display connected

ByteBlock zero= {
  B00000000,
  B00011000,
  B00100100,
  B01000010,
  B01000010,
  B00100100,
  B00011000,
  B00000000
};

ByteBlock one= {
  B00000000,
  B00011100,
  B00101100,
  B01001100,
  B00001100,
  B00001100,
  B00001100,
  B00000000
};

ByteBlock two= {
  B00000000,
  B00111000,
  B01101100,
  B00011000,
  B00110000,
  B01100000,
  B01111110,
  B00000000
};

ByteBlock three= {
  B00000000,
  B00111100,
  B01100110,
  B00001100,
  B00000110,
  B01100110,
  B00111100,
  B00000000
};

ByteBlock four= {
  B00000000,
  B01100000,
  B01100000,
  B01101000,
  B01111110,
  B00001000,
  B00001000,
  B00000000
};

ByteBlock five= {
  B00000000,
  B01111110,
  B01100000,
  B01111000,
  B00000110,
  B01100110,
  B00111100,
  B00000000
};

ByteBlock six= {
  B00000000,
  B00001100,
  B00111000,
  B01100000,
  B01111100,
  B01100110,
  B00111100,
  B00000000
};

ByteBlock seven= {
  B00000000,
  B01111110,
  B00000110,
  B00001100,
  B00011000,
  B00110000,
  B01100000,
  B00000000
};

ByteBlock eight= {
  B00000000,
  B00111100,
  B00100100,
  B00011000,
  B01100110,
  B01000010,
  B00111100,
  B00000000
};

ByteBlock nine= {
  B00000000,
  B00111100,
  B01100110,
  B00111110,
  B00000110,
  B00011100,
  B00110000,
  B00000000
};

ByteBlock digits[10];

void setLEDs (unsigned int number) {

  unsigned int places[4];

  for(unsigned int i = 0;i < 4;i++){
    unsigned int divisor = 1;
    for(unsigned int j=0;j < i;j++){
      divisor *= 10;
    }

    places[3-i] = number/divisor % 10;
    lc.displayOnSegment(3-i,digits[places[3-i]]);
  }

}

//sets all rows on all displays to 0
void switchLED(){
  static bool LEDON = false;
  if(LEDON){
    digitalWrite(13, LOW);
  }else{
    digitalWrite(13, HIGH);
  }
  LEDON = !LEDON;
}

void setup(){

  pinMode(13, OUTPUT);
    
  for(unsigned int i = 0; i < 8;i++){
    digits[0][7-i] = lc.reverse(zero[i]);
  }
  for(unsigned int i = 0; i < 8;i++){
    digits[1][7-i] = lc.reverse(one[i]);
  }
  for(unsigned int i = 0; i < 8;i++){
    digits[2][7-i] = lc.reverse(two[i]);
  }
  for(unsigned int i = 0; i < 8;i++){
    digits[3][7-i] = lc.reverse(three[i]);
  }
  for(unsigned int i = 0; i < 8;i++){
    digits[4][7-i] = lc.reverse(four[i]);
  }
  for(unsigned int i = 0; i < 8;i++){
    digits[5][7-i] = lc.reverse(five[i]);
  }
  for(unsigned int i = 0; i < 8;i++){
    digits[6][7-i] = lc.reverse(six[i]);
  }
  for(unsigned int i = 0; i < 8;i++){
    digits[7][7-i] = lc.reverse(seven[i]);
  }
  for(unsigned int i = 0; i < 8;i++){
    digits[8][7-i] = lc.reverse(eight[i]);
  }
  for(unsigned int i = 0; i < 8;i++){
    digits[9][7-i] = lc.reverse(nine[i]);
  }
}

void loop(){

  lc.clearMatrix();
  
  for (unsigned int i = 0; i<10000; i++) {
    delay(500);
    switchLED();
    setLEDs(i);
  }

}
