/**
 * @file LedControllerDemoRocket.ino
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief Using the the LedController to move a rocket
 * @version 0.1
 * @date 2020-12-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */

//wie immer muss zuerst die Bibliothek eingebunden werden
#include "LedController.hpp"

//Diese Pins werden für die SPI übertragung verwendet
//Schau in die Nutzungsanweisung für eine Beschreibung
#define DIN 27
#define CS 26
#define CLK 25

//Dies Gesamtzahl der Segmente
#define Segments 4

//Die verzögerung zwischen Bewegungen
#define delayTime 200

//Diese Zeile erstellt ein unitialisierten LedController.
//Dieser wird dann in der setup Funktion initialisiert.
LedController<Segments,1> lc = LedController<Segments,1>();  

//Das ist mein Pixelart und ist die Raktete die angezeigt wird.
ByteBlock rocket= {
  B00000000,
  B00001111,
  B00111110,
  B11111101,
  B00111110,
  B00001111,
  B00000000,
  B00000000
};

ByteBlock rocketColumns;

//die LED wechselt den Zustand der internen LED
void switchLED(){
  static bool LEDON = false;
  if(LEDON){
    digitalWrite(13, LOW);
  }else{
    digitalWrite(13, HIGH);
  }
  LEDON = !LEDON;
}

void setup(){

  //hier wird der LedController ohne Hardware SPI initialisiert.
  lc.init(DIN,CLK,CS);

  //Hier wird ein Array aus Spalten der Rakete erstellt.
  //Dies wird vewendet, um die Rakete Stück für Stück reinzuschieben
  //Man kann auch rocketColumns = rocket schreiben, aber dann wird die Rakete um 90 Grad gedreht sein.
  rocketColumns = lc.makeColumns(rocket);
 
  //Erlaubt das Schalten der internen LED
  pinMode(13, OUTPUT);
    
}

void loop(){

  lc.clearMatrix();
  
  for(int i = 0;i < 8*(Segments+1);i++){
    delay(delayTime);

    switchLED();

    //falls die Rakete nicht ganz drin ist, wird sie reingeschoben
    if(i < 8){
      lc.moveRight(rocketColumns[i]);   
    }else{
      //in jedem Schritt wird die Rakte nach rechts geschoben
      lc.moveRight();

      delay(delayTime);

      //nach oben/unten schieben bis der obere/untere Rand erriecht ist
      switch(i % 6){
        case(3):
        case(4):
        case(5):
          lc.moveUp();
          break;

        case(0):
        case(1):
        case(2):
          lc.moveDown();
          break;

        default:
          break;
      }
    }
        
  }

  delay(delayTime);

  for(int i = 0;i < 8*(Segments+1);i++){
    delay(delayTime);

    //bei jedem Schritt soll die Led blinken
    switchLED();

    //falls die Rakete nicht ganz drin ist, wird sie reingeschoben
    if(i < 8){
      lc.moveLeft(rocketColumns[i]);   
    }else{
      //in jedem Schritt wird die Rakte nach links geschoben
      lc.moveLeft();

      delay(delayTime);

      //nach oben/unten schieben bis der obere/untere Rand erriecht ist
      switch(i % 6){
        case(3):
        case(4):
        case(5):
          lc.moveUp();
          break;

        case(0):
        case(1):
        case(2):
          lc.moveDown();
          break;

        default:
          break;
      }
    }
        
  }

  delay(delayTime);

}
