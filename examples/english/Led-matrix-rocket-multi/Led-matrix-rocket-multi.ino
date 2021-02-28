/**
 * @file Led-matrix-rocket-multi.ino
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief An exaple for using multiple rows
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "LedController.hpp"

//spi pins if you do not wat to use hardwware spi
#define DIN 27
#define CLK 25

//The CS pins that should be used for the bottom/top row
#define CS_BOTTOM 25
#define CS_TOP 15

//the number of segments per row
#define Segments 4

//the pin number of the builtin led or an external led to signal a clock.
#define LED 13

// Delay between Frames
#define delayTime 200

//creating an LedController which is not initilized.
auto lc = LedController<Segments,2>();

//the pixelart of our beautiful rocket
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

//switches the state of the builtin led
void switchLED() {
  static bool LEDON = false;
  if(LEDON) {
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }
  LEDON = !LEDON;
}

void setup() {

  //creating the configuration of the controller
  controller_configuration<Segments,2> conf;

  //since more than one row is used (without virtual mult row) make sure to set this to 0
  conf.SPI_CS = 0;

  //These are the chip select pins for each row.
  //The bottom row (row 0) is connected to pin 25 and the top row (row 1) is connected to pin 15
  conf.row_SPI_CS[0] = CS_BOTTOM;
  conf.row_SPI_CS[1] = CS_TOP;

  //this enables hardware spi check what pins to use for your board
  conf.useHardwareSpi = true;

  //this enables debug output (nothing should be printed but it helps to fix possible problems with the config)
  conf.debug_output = true;

  //since we use real multi rows this is set to false (this should not matter since SPI_CS is 0 but this is the proper way)
  conf.virtual_multi_row = false;

  //this specifies the transfer speed of the spi interface. If you want to use high values make sure your cables have a good connection
  conf.spiTransferSpeed = 800000;

  //initilizing the LedController with the configuration which we just set
  lc.init(conf);

  //make a array of columns out of the rocket
  //this is needed to shift it in correctly (you can leave this line if you want to)
  rocketColumns = ByteBlock::makeColumns(rocket);

  //enables the builtin Led to have a kind of clock
  pinMode(LED, OUTPUT);

}

//This is basically the same as Led-matrix-rocket.ino
void loop() {
  lc.clearMatrix();

  for(int i = 0; i < 8*(Segments+1); i++) {
    delay(delayTime);

    switchLED();

    if(i < 8) {
      lc.moveRowRight(rocketColumns[i]);
    } else {
      lc.moveRight();

      delay(delayTime);
      if(i%16 < 8) {
        lc.moveDown();
      } else {
        lc.moveUp();
      }
    }

  }

  delay(delayTime);

  for(int i = 0; i < 8*(Segments+1); i++) {
    delay(delayTime);

    switchLED();

    if(i < 8) {
      lc.moveRowLeft(rocketColumns[i]);
    } else {
      lc.moveLeft();

      delay(delayTime);
      if(i%16 < 8) {
        lc.moveDown();
      } else {
        lc.moveUp();
      }
    }

  }

  delay(delayTime);

}
