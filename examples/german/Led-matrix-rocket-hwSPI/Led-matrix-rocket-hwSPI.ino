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

//Da dieses Beispiel einfach Led-matrix-rocket.ino mit hardware SPI ist, schau auf dessen Seite für Details.
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
  //Nur die folgende Zeile unterscheidet sich von dem Beispiel ohne Hardware SPI, da hier nur ein Pin angegeben werden muss.
  lc.init(CS);

  rocketColumns = ByteBlock::makeColumns(rocket);

  pinMode(13, OUTPUT);

  lc.setIntensity(0);

}

//Das ist mehr oder weniger identisch zu Led-matrix-rocket.ino
void loop() {

  lc.clearMatrix();

  for(int dir = 0; dir < 2; dir++) {
    delay(delayTime);
    for(int i = 0; i < 8*(Segments+1); i++) {
      //einmal Bilinken für jeden Durchlauf
      switchLED();

      //rakete reinfliegen lassen falls sie nicht drin ist
      auto in = (i<8) ? rocketColumns[i] : 0x00;

      //fall dir 0 ist nach rechsts sonst nach links
      dir == 0 ? lc.moveRight(in) : lc.moveLeft(in);

      delay(delayTime);

      //hoch- und runterbewegen
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
