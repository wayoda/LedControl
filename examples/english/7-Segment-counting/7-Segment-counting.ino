/**
 * @file 7-Segment-counting.ino
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief counting up on 7-Segment displays
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

//As always include the libraray
#include "LedController.hpp"
//for this example math.h is also needed
#include "math.h"

//the pin where the chip select is connected to
#define CS 22

//There are two Segments that are connected in series
#define Segments 2

//Each segment has two digits enabled.
//This can be any number between 1 and 8 and can be changed
//without needing to change any other line of code
#define digitsPerSegment 2

//offset to switch which Digits will be used for output
//play around with this value
#define positionOffset 2

//delay before incrementing the counter
#define delayTime 500

//the uninitilized controller object
LedController<Segments,1> lc = LedController<Segments,1>();

//This function calculates the largest number that can be displayed
unsigned long long getLargestNumber() {
  return (unsigned long long) pow(10,Segments*digitsPerSegment);
}

//this function sets the Segments to a given number
void setLEDs (unsigned long long number) {
  //the loop is used to split the given number and set the right digit on the Segments
  for(unsigned int i = 0; i < Segments*digitsPerSegment; i++) {
    unsigned long long divisor = 1;
    for(unsigned int j=0; j < i; j++) {
      divisor *= 10;
    }

    byte num = number/divisor % 10;
    lc.setDigit(Segments-i/digitsPerSegment-1,i%digitsPerSegment+positionOffset,num,false);
  }

}

void setup() {
  //just make sure that the config is valid
  static_assert(positionOffset+digitsPerSegment<9,"invalid configuration");

  //initilize a ledcontroller with a hardware spi and one row
  lc.init(CS);

  //disables all Digits by default
  for(unsigned int i = 0; i < Segments; i++) {
    for(unsigned int j = 0; j < 8; j++) {
      lc.setRow(i,j,0x00);
    }
  }

  //Set them to the lowest brightness
  lc.setIntensity(0);
}

void loop() {

  //clear the matrix just to be sure there is nothing on it
  lc.clearMatrix();

  //calculate the highest possible number and set it as end condition for the loop
  auto limit = getLargestNumber();

  //count up and display the number
  for (unsigned long long i = 0; i<limit; i++) {
    setLEDs(i);
    delay(delayTime);
  }

}
