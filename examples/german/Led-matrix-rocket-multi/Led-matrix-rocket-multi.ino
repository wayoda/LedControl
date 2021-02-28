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

//spi pins falls nicht hardware SPI verwendet werden soll
#define DIN 27
#define CLK 25

//Die CS pins die für die untere/obere Zeile verwendet werden sollen
#define CS_BOTTOM 25
#define CS_TOP 15

//Die Anzahl der Segmente pro Zeile
#define Segments 4

//Der Pin der internen LED
#define LED 13

//Die verzögerung zwischen zwei Bewegungen
#define delayTime 200

//Hier wird wieder ein uninitialisierter LedController erstellt
auto lc = LedController<Segments,2>();

//Das Pixelart der wunderschönen Rakete
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

//schaltet den Zustand der internen LED um
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

  //Erstellen einer Konfiguration für den LedController
  controller_configuration<Segments,2> conf;

  //Da mehr als eine Zeile verwendet werden soll (ohne virtual multi Row) sollte das 0 sein.
  conf.SPI_CS = 0;

  //hier werden die Chip Select Pins der einzelnen Reihen gesetzt.
  //Die untere Zeile (Zeile 0) ist an Pin 25 geschlossen und die obere Reihe (Reihe 1) an Pin 15
  conf.row_SPI_CS[0] = CS_BOTTOM;
  conf.row_SPI_CS[1] = CS_TOP;

  //Das setzt die nńutzung von harware SPI
  //Schaue welche Pins bei deinem Board verwendet werden müssen.
  conf.useHardwareSpi = true;

  //Das schaltet die Debug Ausgabe an.
  //Es sollte zwar nichts ausgegeben werden, kann aber trotzdem hilfreich sein.
  conf.debug_output = true;

  //Da wir virtual_multi_row nicht verwenden sollte es auf false gesetzt werden.
  //Es sollte zwar egal sein, da SPI_CS 0 ist aber sicher ist sicher.
  conf.virtual_multi_row = false;

  //Das setzt die Übertragungsgeschwindigkeit der SPI Schnittstelle.
  //Falls hohe Werte gesetzt werden, sollte eine gute Verbingung sichergestellt werden.
  conf.spiTransferSpeed = 800000;

  //Initialisiere den LedController mit der erstellten Konfiguration.
  lc.init(conf);

  //Hier wird ein Array aus Spalten der Rakete erstellt.
  //Dies wird vewendet, um die Rakete Stück für Stück reinzuschieben
  //Man kann auch rocketColumns = rocket schreiben, aber dann wird die Rakete um 90 Grad gedreht sein.
  rocketColumns = ByteBlock::makeColumns(rocket);

  //Erlaubt das steuern der internen LED
  pinMode(LED, OUTPUT);

}

//Das ist mehr oder weniger identisch zu Led-matrix-rocket.ino
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
