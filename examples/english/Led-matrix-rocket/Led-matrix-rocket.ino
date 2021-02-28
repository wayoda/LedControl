/**
 * @file Led-matrix-rocket.ino
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief Using the the LedController to move a rocket
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

//as always include the LedController header on the top
#include "LedController.hpp"

//These are the Pins used for the SPI transfer
//See the usage instructions for their meaning
#define DIN 27
#define CS 26
#define CLK 25

//The total numer of Segments
#define Segments 4

//The delay between movements
#define delayTime 200

//This creates an uninitilized LedController object.
//It will be initilized in the setup function.
LedController<Segments,1> lc = LedController<Segments,1>();

//This is my pixelart of a rocket which will be used in this example
ByteBlock rocket= ByteBlock::reverse({
  B00000000,
  B00001111,
  B00111110,
  B11111101,
  B00111110,
  B00001111,
  B00000000,
  B00000000
});

ByteBlock rocketColumns;

//switches the state of the internal LED
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

  //initilizes the LedController without hardware spi.
  lc.init(DIN,CLK,CS);

  //make a array of columns out of the rocket
  //this is needed to shift it in correctly (you can leave this line if you want to)
  rocketColumns = ByteBlock::makeColumns(rocket);

  //enable the LED to have a clock
  pinMode(13, OUTPUT);

}

void loop() {

  lc.clearMatrix();

  for(int dir = 0; dir < 2; dir++) {
    delay(delayTime);
    for(int i = 0; i < 8*(Segments+1); i++) {
      //blink led for each iteration
      switchLED();

      //if rocket not fully inside let it fly in
      auto in = (i<8) ? rocketColumns[i] : 0x00;

      //if dir is 0 move right if not move left
      dir == 0 ? lc.moveRight(in) : lc.moveLeft(in);

      delay(delayTime);

      //decide whether to move up or down
      if(i > 7) {
        if(i % 6 < 3) {
          lc.moveDown();
        } else {
          lc.moveUp();
        }
      }

      delay(delayTime);

    }
  }

}
