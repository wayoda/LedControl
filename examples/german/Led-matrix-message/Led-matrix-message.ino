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

//Wie immer die Bibliothek einbinden
#include "LedController.hpp"

//Der Pin, der mit CS der Matrix verbunden ist.
#define CS 22

//Die Gesamtzahl der Segments
//Alle sind in einer Reihe und der Wert kann frei geändert werden.
#define Segments 4

//Die Verzögerung bis der Inhalt wieder um eins bewegt wird.
#define delayTime 200

//Das uninitialisierte LedController Objekt
LedController<Segments,1> lc = LedController<Segments,1>();

// Zeichen und Buchstaben ---------------------------------------------------------------------------------
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

//Die Anzahl der ByteBlocks aus der die Nachricht besteht.
const unsigned int message_length = 6;

//Dieses Array definiert die Nachricht, welche auf der Matrix angezeigt wird.
//Standardmäßig wird 'Jahallo!! :heart:' ausgegeben, kann aber frei geändert werden.
ByteBlock message[message_length] = {
  J&(a>>5), //'J' und erste Hälfte von 'a'
  (a<<3)&(h>>2), // zweite Hälfte von 'a' und 'h'
  (a<<1)&(l>>5), // 'a' und 'l'
  l&(o>>3), // 'l' und 'o'
  (AZ<<1)&(AZ>>1)&((++Heart)>>5), // ! und ! und erste Hälfte von ':heart:'
  (++Heart)<<4 // zweite Hälfte von ':heart:' (ändere die 4 zu einer 3 um das ganze Herz anzuzeigen)
};


void setup() {
  //erstelle eine Konfiguration mit der gleichen Größe wie der Controller
  controller_configuration<Segments,1> conf;
  //Nutze den CS pin
  conf.SPI_CS = CS;
  //Setzt die Übertragungsgeschwindigkeit auf den höchsten stabilen Wert
  conf.spiTransferSpeed = 10000000;
  //schaltet Hardware SPI an
  conf.useHardwareSpi = true;

  //initialisiere den Controller mit der Konfiguratin
  lc.init(conf);

  //setzte die Helligkeit auf das niedrigste
  lc.setIntensity(0);
}

void loop() {
  //sicherheitshalber Matrix leeren
  lc.clearMatrix();

  //Daten reinschieben
  //Jedes Segment der Nachricht wird einzeln reingeschoben
  for(unsigned int i = 0; i < message_length; i++) {
    //Da jedes Segment eine Breite von 8 pixeln hat noch eine Schleife
    for(unsigned int j = 0; j < 8; j++) {
      //Schiebt alles nach links und fügt rechts eine Spalte der Nachricht rein
      lc.moveLeft(ByteBlock::makeColumns(message[i])[j]);
      delay(delayTime);
    }
  }

  //schiebt die daten raus
  //Da die größe der Matrix 8*Segments ist muss einfach so oft geschoben werden.
  for(unsigned int i = 0; i < 8*Segments; i++) {
    lc.moveLeft();
    delay(delayTime);
  }
}
