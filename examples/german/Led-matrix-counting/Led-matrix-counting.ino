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

//Wie immer muss zuerst die Library verwendet werden
#include "LedController.hpp"

//Der Pin an den das Chip Select signal der Matrix angeschlossen ist
#define CS 15

//Die Anzahl der Segmente in der einen Reihe
#define Segments 4

//Die Zeitverzögerung bis eins weiter gezählt wird
#define delayTime 200

//Das ist die Variable in der das verwendete Objekt gespeichert wird.
LedController<Segments,1> lc;

//Dieses Array definiert nur wie die Ziffern aussehen.
ByteBlock digits[10] = {
  {
    B00000000,
    B00011000,
    B00100100,
    B01000010,
    B01000010,
    B00100100,
    B00011000,
    B00000000
  }, {
    B00000000,
    B00011100,
    B00101100,
    B01001100,
    B00001100,
    B00001100,
    B00001100,
    B00000000
  }, {
    B00000000,
    B00111000,
    B01101100,
    B00011000,
    B00110000,
    B01100000,
    B01111110,
    B00000000
  }, {
    B00000000,
    B00111100,
    B01100110,
    B00001100,
    B00000110,
    B01100110,
    B00111100,
    B00000000
  }, {
    B00000000,
    B01100000,
    B01100000,
    B01101000,
    B01111110,
    B00001000,
    B00001000,
    B00000000
  }, {
    B00000000,
    B01111110,
    B01100000,
    B01111000,
    B00000110,
    B01100110,
    B00111100,
    B00000000
  }, {
    B00000000,
    B00001100,
    B00111000,
    B01100000,
    B01111100,
    B01100110,
    B00111100,
    B00000000
  }, {
    B00000000,
    B01111110,
    B00000110,
    B00001100,
    B00011000,
    B00110000,
    B01100000,
    B00000000
  }, {
    B00000000,
    B00111100,
    B00100100,
    B00011000,
    B01100110,
    B01000010,
    B00111100,
    B00000000
  }, {
    B00000000,
    B00111100,
    B01100110,
    B00111110,
    B00000110,
    B00011100,
    B00110000,
    B00000000
  }
};

//Diese Funktion zeigt einene gegebene Nummer auf der Matrix an
void setLEDs (unsigned int number) {
  //Die Schleife zerlegt die Nummer in die einzalnen Ziffern
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

//Diese Funktion lässt die interne LED blinken
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

  //Hier wird ein LedController mit Hardware SPI erstelle und lc zugewiesen
  lc = LedController<Segments,1>(CS);

  //Der Pin für die interne LED wird auf output gesetzt
  pinMode(13, OUTPUT);

  //alle Ziffern müssen um 180 Grad gedreht werden, um sie richtig anzuzeigen.
  //man kann das weglassen, aber dann sine die Ziffern falsch
  for(unsigned int i = 0; i < 10; i++) {
    digits[i] = ByteBlock::rotate180(digits[i]);
  }

}

void loop() {

  //erst wird die Matrix geleert
  lc.clearMatrix();

  //In dieser Schleife wird einfach gezählt und jeweils die nummer angezeigt
  for (unsigned int i = 0; i<10000; i++) {
    delay(500);
    switchLED();
    setLEDs(i);
  }

}
