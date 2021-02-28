/**
 * @file Led-matrix-message.ino
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief This demo shows how a message can be displayed on an LedMatrix.
 * This demo is based on https://www.meine-schaltung.de/schaltung/soft/anzeige/vierfach_matrix_mit_arduino/ and modified for the latest LedController.
 * @version 0.1
 * @date 2021-02-16
 *
 * @copyright Copyright (c) 2021
 *
 */

//As always include the library
#include "LedController.hpp"

//The pin connected to the CS pin of the Matrix
#define CS 22

//the total number of segments.
//In this example they are in one row and you can freely change this number.
#define Segments 4

//The delay time until the contents move left by one.
#define delayTime 200

//The ledController object, at the moment it is uninitilized
LedController<Segments,1> lc = LedController<Segments,1>();

// Symbols and Letters ---------------------------------------------------------------------------------
ByteBlock A =  ByteBlock::rotate180({ B00000000, B00011000, B00100100, B00100100, B00111100, B00100100, B00100100, B00000000 });
ByteBlock a =  ByteBlock::rotate180({ B00000000, B00000000, B00111000, B01001000, B01001000, B01001000, B00111000, B00000000 });
ByteBlock b =  ByteBlock::rotate180(ByteBlock::reverse({ B00000000, B00001000, B00111000, B01001000, B01001000, B01001000, B00111000, B00000000 }))<<2;
ByteBlock c =  ByteBlock::rotate180({ B00000000, B00000000, B00110000, B01000000, B01000000, B01000000, B00110000, B00000000 });
ByteBlock d =  ByteBlock::rotate180({ B00000000, B00001000, B00111000, B01001000, B01001000, B01001000, B00111000, B00000000 });
ByteBlock e =  ByteBlock::rotate180({ B00000000, B00000000, B00110000, B01001000, B01111000, B01000000, B00111000, B00000000 });
ByteBlock H =  ByteBlock::rotate180({ B00000000, B10010000, B10010000, B11110000, B10010000, B10010000, B10010000, B00000000 });
ByteBlock h =  ByteBlock::rotate180({ B00000000, B01000000, B01110000, B01001000, B01001000, B01001000, B01001000, B00000000 });
ByteBlock I =  ByteBlock::rotate180({ B00000000, B01000000, B01000000, B01000000, B01000000, B01000000, B01000000, B00000000 });
ByteBlock i =  ByteBlock::rotate180({ B00000000, B10000000, B00000000, B10000000, B10000000, B10000000, B10000000, B00000000 });
ByteBlock J =  ByteBlock::rotate180({ B00000000, B00001000, B00001000, B00001000, B00001000, B01001000, B00110000, B00000000 });
ByteBlock K =  ByteBlock::rotate180({ B00000000, B10001000, B10010000, B11100000, B10100000, B10010000, B10001000, B00000000 });
ByteBlock k =  ByteBlock::rotate180({ B00000000, B10000000, B10010000, B10100000, B11000000, B10100000, B10010000, B00000000 });
ByteBlock l =  ByteBlock::rotate180({ B00000000, B10000000, B10000000, B10000000, B10000000, B10000000, B01000000, B00000000 });
ByteBlock M =  ByteBlock::rotate180({ B00000000, B01000100, B01101100, B01010100, B01000100, B01000100, B01000100, B00000000 });
ByteBlock m =  ByteBlock::rotate180({ B00000000, B00000000, B01110110, B01001001, B01001001, B01001001, B01001001, B00000000 });
ByteBlock n =  ByteBlock::rotate180({ B00000000, B00000000, B01110000, B01001000, B01001000, B01001000, B01001000, B00000000 });
ByteBlock o =  ByteBlock::rotate180({ B00000000, B00000000, B01100000, B10010000, B10010000, B10010000, B01100000, B00000000 });
ByteBlock p =  ByteBlock::rotate180({ B00000000, B00000000, B11100000, B10001000, B10001000, B10001000, B11100000, B10000000 });
ByteBlock q =  ByteBlock::rotate180(ByteBlock::reverse({ B00000000, B00000000, B11100000, B10001000, B10001000, B10001000, B11100000, B10000000 }))<<2;
ByteBlock r =  ByteBlock::rotate180({ B00000000, B00000000, B10100000, B11000000, B10000000, B10000000, B10000000, B00000000 });
ByteBlock s =  ByteBlock::rotate180({ B00000000, B00000000, B01110000, B10000000, B01100000, B00010000, B11100000, B00000000 });
ByteBlock t =  ByteBlock::rotate180({ B00000000, B10000000, B11100000, B10000000, B10000000, B10000000, B01100000, B00000000 });
ByteBlock u =  ByteBlock::rotate180({ B00000000, B00000000, B10010000, B10010000, B10010000, B10010000, B01100000, B00000000 });
ByteBlock w =  ByteBlock::rotate180({ B00000000, B00000000, B10001000, B10001000, B10101000, B11011000, B10001000, B00000000 });
ByteBlock LB = ByteBlock::rotate180({ B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 });
ByteBlock AZ = ByteBlock::rotate180({ B01000000, B01000000, B01000000, B01000000, B01000000, B00000000, B01000000, B00000000 });

ByteBlock Heart = ByteBlock::rotate180({
  B00000000,
  B01000010,
  B10100101,
  B10011001,
  B10000001,
  B01000010,
  B00100100,
  B00011000,
});

//The number of ByteBlocks the message consists of.
const unsigned int message_length = 6;

//This array defines the message that will be displayed on the matrix.
//By default it says 'Jahallo!! :heart:' and can be modified however needed.
ByteBlock message[message_length] = {
  J&(a>>5), //'J' and first half of 'a'
  (a<<3)&(h>>2), // second half of 'a' and 'h'
  (a<<1)&(l>>5), // 'a' and 'l'
  l&(o>>3), // 'l' and 'o'
  (AZ<<1)&(AZ>>1)&((++Heart)>>5), // ! and ! and first half of ':heart:'
  (++Heart)<<4 // second half of ':heart:' (change the 4 to a 3 if you want to see the complete heart)
};


void setup() {
  //setup the config with the same size as the controller
  controller_configuration<Segments,1> conf;
  //use the specified CS pin
  conf.SPI_CS = CS;
  //set the transfer speed to the highest stable value
  conf.spiTransferSpeed = 10000000;
  //enable hardware spi
  conf.useHardwareSpi = true;

  //init the controller from the configuration
  lc.init(conf);

  //set the brightness as low as possible
  lc.setIntensity(0);
}

void loop() {
  //clear just to be safe
  lc.clearMatrix();

  //shift data in
  //each segment of the message will be shifted in one by one
  for(unsigned int i = 0; i < message_length; i++) {
    //since each segment has a width of 8 pixels there is another loop
    for(unsigned int j = 0; j < 8; j++) {
      //move the columns in one by one and wait a bit
      lc.moveLeft(ByteBlock::makeColumns(message[i])[j]);
      delay(delayTime);
    }
  }

  //shift the contents out
  //since the sice is 8*segments simply shift left that many times
  for(unsigned int i = 0; i < 8*Segments; i++) {
    lc.moveLeft();
    delay(delayTime);
  }
}
