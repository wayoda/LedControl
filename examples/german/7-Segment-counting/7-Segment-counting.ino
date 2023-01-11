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

//Wie immer die Bibliothek einbinden
#include "LedController.hpp"
//in diesem Beispiel wird zusätzlich math-h benötigt
#include "math.h"

//Der Pin an dem CS angeschlossen ist
#define CS 22

//Die Anzahl der in Reihe geschlossenen Segmente
#define Segments 2

//Die Anzahl der Ziffern pro Segment.
//Der Wert kann aller von 1 bis 8 sein und frei geändert werden.
#define digitsPerSegment 2

//Eine Verschiebung, um auszuwählen auf welche Ziffern ausgegeben wird.
//probier einfach ein paar Werte aus.
#define positionOffset 2

//Verzögerung vor dem Erhöhen des Zählers
#define delayTime 500

//Das uninitialisierte controller Objekt
LedController<Segments,1> lc = LedController<Segments,1>();

//Diese Funktion berechnet die größte Zahl, die angezeigt werden kann
unsigned long long getLargestNumber() {
  return (unsigned long long) pow(10,Segments*digitsPerSegment);
}

//Diese Funktion setzt die Anzeige auf eine gegebene Zahl
void setLEDs (unsigned long long number) {
  //Diese Schleife zerlegt die Zahl und setzt die einzelnen Ziffern
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
  //Sicherstellen, dass die Konfiguration gültig ist
  static_assert(positionOffset+digitsPerSegment<9,"invalid configuration");

  //initialisieren des LedController Objektes mit hardware SPI und einer Reihe
  lc.init(CS);

  //alle Segmente sind standardmäßig ausgeschaltet
  for(unsigned int i = 0; i < Segments; i++) {
    for(unsigned int j = 0; j < 8; j++) {
      lc.setRow(i,j,0x00);
    }
  }

  //Helligkeit auf das minimum setzten
  lc.setIntensity(0);
}

void loop() {

  //Sicherheitshalber die Ausgabe zurücksetzten
  lc.clearMatrix();

  //bestimmen der größten Zahl
  auto limit = getLargestNumber();

  //Hochzählen und anzeigen der Zahl
  for (unsigned long long i = 0; i<limit; i++) {
    setLEDs(i);
    delay(delayTime);
  }

}
