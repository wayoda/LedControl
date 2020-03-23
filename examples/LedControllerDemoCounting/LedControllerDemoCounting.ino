#include "LedController.hpp"

#define DIN 27
#define CS 26
#define CLK 25

#define Segments 4

#define delayTime 200 // Delay between Frames

LedController lc = LedController(DIN,CLK,CS,Segments);  // Pins: DIN,CLK,CS, # of Display connected

std::array<byte,8> zero= {
  B00000000,
  B00011000,
  B00100100,
  B01000010,
  B01000010,
  B00100100,
  B00011000,
  B00000000
};

std::array<byte,8> one= {
  B00000000,
  B00011100,
  B00101100,
  B01001100,
  B00001100,
  B00001100,
  B00001100,
  B00000000
};

std::array<byte,8> two= {
  B00000000,
  B00111000,
  B01101100,
  B00011000,
  B00110000,
  B01100000,
  B01111110,
  B00000000
};

std::array<byte,8> three= {
  B00000000,
  B00111100,
  B01100110,
  B00001100,
  B00000110,
  B01100110,
  B00111100,
  B00000000
};

std::array<byte,8> four= {
  B00000000,
  B01100000,
  B01100000,
  B01101000,
  B01111110,
  B00001000,
  B00001000,
  B00000000
};

std::array<byte,8> five= {
  B00000000,
  B01111110,
  B01100000,
  B01111000,
  B00000110,
  B01100110,
  B00111100,
  B00000000
};

std::array<byte,8> six= {
  B00000000,
  B00001100,
  B00111000,
  B01100000,
  B01111100,
  B01100110,
  B00111100,
  B00000000
};

std::array<byte,8> seven= {
  B00000000,
  B01111110,
  B00000110,
  B00001100,
  B00011000,
  B00110000,
  B01100000,
  B00000000
};

std::array<byte,8> eight= {
  B00000000,
  B00111100,
  B00100100,
  B00011000,
  B01100110,
  B01000010,
  B00111100,
  B00000000
};

std::array<byte,8> nine= {
  B00000000,
  B00111100,
  B01100110,
  B00111110,
  B00000110,
  B00011100,
  B00110000,
  B00000000
};

std::array <byte ,8> getDigit(unsigned int i){
  switch (i){
  case 0:
    return lc.rotate180(zero);
  case 1:
    return lc.rotate180(one);
    case 2:
      return lc.rotate180(two);
    case 3:
      return lc.rotate180(three);
    case 4:
      return lc.rotate180(four);
    case 5:
      return lc.rotate180(five);
    case 6:
      return lc.rotate180(six);
    case 7:
      return lc.rotate180(seven);
    case 8:
      return lc.rotate180(eight);
    case 9:
      return lc.rotate180(nine);
  
  default:
    return lc.rotate180(zero);
  }
}

void setLEDs (unsigned int number) {
  unsigned int Stelle3 = number/1 %10;
  unsigned int Stelle2 = number/10 %10;
  unsigned int Stelle1 = number/100 %10;
  unsigned int Stelle0 = number/1000 % 10;  

  lc.displayOnSegment(0,getDigit(Stelle0).data());
  lc.displayOnSegment(1,getDigit(Stelle1).data());
  lc.displayOnSegment(2,getDigit(Stelle2).data());
  lc.displayOnSegment(3,getDigit(Stelle3).data());
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
    
}

void loop(){
  
  for (unsigned int i = 0; i<10000; i++) {
    delay(500);
    switchLED();
    setLEDs(i);
  }

}
