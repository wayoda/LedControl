//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** Check pin numbers will probably not work with your hardware *****
 We have only a single MAX72XX.
 */
#define TRACE_ON

#define LC_DATA_PIN 8
#define LC_LOAD_PIN 7
#define LC_CLK_PIN 6
#define LC_MODULE_COUNT 1

LedControl lc=LedControl(LC_DATA_PIN,LC_CLK_PIN,LC_LOAD_PIN,LC_MODULE_COUNT);
bool rotate=false;

/* we always wait a bit between updates of the display */
unsigned long delaytime=500;

void setup() {
  #ifdef TRACE_ON 
    char compile_signature[] = "--- START (Build: " __DATE__ " " __TIME__ ") ---";   
    Serial.begin(9600);
    Serial.println(compile_signature); 
  #endif
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}

/* Loop over demonstration sequences. Comment unnecessary if u like*/
void loop() { 
 
  writeArduinoOn7Segment(); // expects 1 digit
  scrollDigits(); // expects 4 digits
  writeArduinoString(); // expects 8 digits
  writeGoStringOnPositions(); // expects 8 digits
  traverseCodepage(); // expects 8 digits
  rotate=!rotate;
  lc.setRotate180(0,rotate);
}


/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */

void writeArduinoOn7Segment() {
  lc.clearDisplay(0);
  lc.setChar(0,0,'a',false);
  delay(delaytime);
  lc.setChar(0,0,'r',false);
  delay(delaytime);
  lc.setChar(0,0,'d',false);
  delay(delaytime);
  lc.setChar(0,0,'u',false);
  delay(delaytime);
  lc.setChar(0,0,'i',false);
  delay(delaytime);
  lc.setChar(0,0,'n',false);
  delay(delaytime);
  lc.setChar(0,0,'o',false);
  delay(delaytime);
  lc.clearDisplay(0);
  delay(delaytime);
} 

/*
 This method will display the String 
 "Arduino" on a 8 digit 7 Seg display 
 */
void writeArduinoString() {
  lc.setString(0,7,"Arduino",0x02);
  delay(delaytime*5);
}

/*
 This method will display the String 
 "Go" at different locations and demonstrate how the other digirs are preseved 
 */

void writeGoStringOnPositions() {
  lc.setString(0,7,"________",0x55);
  for(int i=1;i<8;i++)
  {
    lc.setString(0,i,"Go",0x80);
    delay(delaytime);
  }
}

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits() {
  for(int i=0;i<13;i++) {
    lc.setDigit(0,3,i,false);
    lc.setDigit(0,2,i+1,false);
    lc.setDigit(0,1,i+2,false);
    lc.setDigit(0,0,i+3,false);
    delay(delaytime);
  }
  lc.clearDisplay(0);
  delay(delaytime);
}

/*
  This method will travers over the whole codepage in 4 byte steps
  showing the characters on the rightside and the offset in decimal on the left
  */

void traverseCodepage()
{
  String offsetAsString;
  lc.clearDisplay(0);
  lc.setString(0,7,"Codepage",0);
  delay(delaytime*2);
  lc.clearDisplay(0);
  for (int i=0;i<128;i+=4) {
    offsetAsString=String(i);
    lc.setString(0,4+offsetAsString.length(),offsetAsString,0);
    lc.setChar(0,3,i,false);
    lc.setChar(0,2,i+1,false);
    lc.setChar(0,1,i+2,false);
    lc.setChar(0,0,i+3,false);
    delay(delaytime*4);
  }
}


