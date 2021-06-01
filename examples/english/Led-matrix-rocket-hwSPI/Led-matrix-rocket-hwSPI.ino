/**
 * @file Led-matrix-rocket-hwSPI.ino
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief Led-matrix-rocket.ino with hardware spi
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

//Since this is just Led-matrix-rocket.ino with hardware SPI, view its site for more details.
#include "LedController.hpp"

#define CS 15
#define Segments 4

#define delayTime 200

LedController<Segments,1> lc = LedController<Segments,1>();

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
  //Only the following line is different from the example without hardware SPI since in this case only one Pin needs to be specified
  lc.init(CS);

  rocketColumns = ByteBlock::makeColumns(rocket);

  pinMode(13, OUTPUT);

  lc.setIntensity(0);

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
