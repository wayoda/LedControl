/**
 * @file Led-matrix-counting.ino
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief counting up on an led matrix
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "LedController.hpp"

//the pin where the chip select is connected to
#define CS 15

#define Segments 4

#define delayTime 200 // Delay between Frames

LedController<Segments,1> lc;

//these are just some digits to display numbers on the matrix
ByteBlock digits[10] = {
  {
    0b00000000,
    0b00011000,
    0b00100100,
    0b01000010,
    0b01000010,
    0b00100100,
    0b00011000,
    0b00000000
  }, {
    0b00000000,
    0b00011100,
    0b00101100,
    0b01001100,
    0b00001100,
    0b00001100,
    0b00001100,
    0b00000000
  }, {
    0b00000000,
    0b00111000,
    0b01101100,
    0b00011000,
    0b00110000,
    0b01100000,
    0b01111110,
    0b00000000
  }, {
    0b00000000,
    0b00111100,
    0b01100110,
    0b00001100,
    0b00000110,
    0b01100110,
    0b00111100,
    0b00000000
  }, {
    0b00000000,
    0b01100000,
    0b01100000,
    0b01101000,
    0b01111110,
    0b00001000,
    0b00001000,
    0b00000000
  }, {
    0b00000000,
    0b01111110,
    0b01100000,
    0b01111000,
    0b00000110,
    0b01100110,
    0b00111100,
    0b00000000
  }, {
    0b00000000,
    0b00001100,
    0b00111000,
    0b01100000,
    0b01111100,
    0b01100110,
    0b00111100,
    0b00000000
  }, {
    0b00000000,
    0b01111110,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00110000,
    0b01100000,
    0b00000000
  }, {
    0b00000000,
    0b00111100,
    0b00100100,
    0b00011000,
    0b01100110,
    0b01000010,
    0b00111100,
    0b00000000
  }, {
    0b00000000,
    0b00111100,
    0b01100110,
    0b00111110,
    0b00000110,
    0b00011100,
    0b00110000,
    0b00000000
  }
};

//this function sets the matrix to a given number
void setLEDs (unsigned int number) {
  //the loop is used to split the given number and set the right digit on the matix
  unsigned int places[Segments];

  for(unsigned int i = 0; i < Segments; i++) {
    unsigned int divisor = 1;
    for(unsigned int j=0; j < i; j++) {
      divisor *= 10;
    }

    places[Segments-1-i] = number/divisor % 10;
    lc.displayOnSegment(Segments-1-i,digits[places[Segments-1-i]]);
  }

}

//this function switches a led to have a
void switchLED() {
  static bool LEDON = false;
  if(LEDON) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  LEDON = !LEDON;
}

void setup() {

  //create a ledcontroller with a hardware spi and one row
  lc = LedController<Segments,1>(CS);

  //enable the led
  pinMode(13, OUTPUT);

  //rotate all digits by 180 degress to display them correctly
  //you could leave this but then the orientation would be wrong
  for(unsigned int i = 0; i < 10; i++) {
    digits[i] = digits[i].rotate180();
  }

}

void loop() {

  //clear the matrix just to be sure there is nothing on it
  lc.clearMatrix();

  //count up and display the number
  for (unsigned int i = 0; i<10000; i++) {
    delay(500);
    switchLED();
    setLEDs(i);
  }

}
