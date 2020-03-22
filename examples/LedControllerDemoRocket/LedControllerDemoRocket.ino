#include "LedController.hpp"

#define DIN 27
#define CS 26
#define CLK 25

#define Segments 4

#define delayTime 200 // Delay between Frames

LedController lc = LedController(DIN,CLK,CS,Segments);  // Pins: DIN,CLK,CS, # of Display connected

std::array<byte,8> rocket= {
  B00000000,
  B00001111,
  B00111110,
  B11111101,
  B00111110,
  B00001111,
  B00000000,
  B00000000
};

auto rocketColumns = lc.makeColumns(rocket);

//sets all rows on all displays to 0
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

  pinMode(13, OUTPUT);
    
}

void loop(){
  
    lc.clearMatrix();
    
    //Let the rocket fly in
    for(int i = 0;i < 8*(Segments+1);i++){
      delay(delayTime);

      //blink led for each iteration
      switchLED();

      //if rocket not fully inside let it fly in and shift it
      if(i < 8){
        lc.moveLeft(rocketColumns[i]);   
      }else{
        lc.moveLeft();

        delay(delayTime);
        if(i%6 == 4){
          lc.moveUp();
        }else if(i%6 == 0){
          lc.moveUp();
        }else if(i%6 == 1){
          lc.moveDown();
        }else if(i%6 == 2){
          lc.moveDown();
        }else if(i%6 == 3){
          lc.moveDown();
        }else{
          lc.moveUp();
        }
      }
         
    }

    delay(delayTime);

    for(int i = 0;i < 8*(Segments+1);i++){
      delay(delayTime);

      //blink led for each iteration
      switchLED();

      //if rocket not fully inside let it fly in and shift it
      if(i < 8){
        lc.moveRight(rocketColumns[i]);   
      }else{
        lc.moveRight();

        delay(delayTime);
        if(i%6 == 4){
          lc.moveUp();
        }else if(i%6 == 0){
          lc.moveUp();
        }else if(i%6 == 1){
          lc.moveDown();
        }else if(i%6 == 2){
          lc.moveDown();
        }else if(i%6 == 3){
          lc.moveDown();
        }else{
          lc.moveUp();
        }
      }
         
    }

    delay(delayTime);

}
