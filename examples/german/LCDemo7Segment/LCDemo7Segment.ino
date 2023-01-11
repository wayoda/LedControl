/**
 * @file LCDemo7Segment.ino
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief using the ledcontroller with 7-segment displays
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

//Wie immer muss am Anfang die Bibliothek eingebunden werden.
#include "LedController.hpp"

/*
 Die folgenden 3 Variabeln müssen abhängig vom board ggf geändert werden.
 pin 15 ist verbunden mit DataIn
 pin 14 ist verbunden mit CLK
 pin 13 ist verbunden mit LOAD/ChipSelect
*/
#define DIN 15
#define CLK 14
#define CS 13


//Nun wird die Variable erstellt in der der LedController mit nur einem Segment(nur ein MAX72XX) gespeichert wird.
LedController<1,1> lc;

//Die verzögerung zwischen updates des Displays
unsigned long delaytime=250;

void setup() {

  //Nun wird das LedController Objekt ohne Hardware SPI erstellt.
  lc=LedController<1,1>(DIN,CLK,CS);

  //Setzt die Helligkeit auf einen Mittleren Wert
  lc.setIntensity(8);

  //leere den Inhalt des Segments
  lc.clearMatrix();
}

//Diese Funktion wird das Wort Arduino an Stelle 0 buchstabiert anzeigen.
void writeArduinoOn7Segment() {
  lc.setChar(0,0,'a',false);
  delay(delaytime);
  lc.setRow(0,0,0x05);
  delay(delaytime);
  lc.setChar(0,0,'d',false);
  delay(delaytime);
  lc.setRow(0,0,0x1c);
  delay(delaytime);
  lc.setRow(0,0,B00010000);
  delay(delaytime);
  lc.setRow(0,0,0x15);
  delay(delaytime);
  lc.setRow(0,0,0x1D);
  delay(delaytime);
  lc.clearMatrix();
  delay(delaytime);
}

//Diese Funktion wird alle Hexadezimalziffern entlang des Displays bewegen.
//Es werden mindestens vier 7-Segment Elemente benötigt damit die Anzeige gut aussieht.
void scrollDigits() {
  for(int i=0; i<13; i++) {
    lc.setDigit(0,3,i,false);
    lc.setDigit(0,2,i+1,false);
    lc.setDigit(0,1,i+2,false);
    lc.setDigit(0,0,i+3,false);
    delay(delaytime);
  }
  lc.clearMatrix();
  delay(delaytime);
}

void loop() {
  writeArduinoOn7Segment();
  scrollDigits();
}
