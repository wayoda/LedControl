#include <LedControl.h>  
  
int DIN = 12;  
int CS =  11;  
int CLK = 10; 
 
byte arrow[8]=  {0x18,0x3C,0x5A,0x99,0x18,0x18,0x18,0x24};
byte love[8]=   {0x00,0x46,0xB9,0x81,0x42,0x24,0x18,0x00};
byte smile[8]=  {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};

LedControl lc=LedControl(DIN,CLK,CS,4);  
  
void setup()
{  
    for (int i = 0; i <= 3; i++) {
        lc.shutdown(i,false);   
        lc.setIntensity(i, 3);     
        lc.clearDisplay(i);         
    }
}  
  
void loop()
{      
    // Display moving graphics
    // Passing an array of graphics through parameters :)
    moving(arrow);
    moving(love);
    moving(smile);
}  

void moving (byte array_date [])
{
    printByte_in(array_date, 0);
    for (int i = 0; i <= 3; i++) 
        printByte_zhong(array_date, i);
}

void printByte_in(byte character [], int addr)  
{  
    int i = 0, column = 0, value = 0;
    for (i = 0; i <= 7; i++) {
        value = -1;
        for (column = 7 - i; column <= 7; column++) {
            value++;  
            lc.setColumn(addr,column,character[value]); 
        }  
        delay(40);
    }
}

void printByte_zhong(byte character [], int addr)  
{
    int sum = 0, value = 0, character_value = 0, range = 0;
    int i = 0, j = 0;
    
    for (i = 7, range = 0, value = 1; i >= 0; i--, range++, value++) {
        for (j = 7 - range, character_value = 0; j <= 7; j++, character_value++)
            lc.setColumn(addr + 1, j, character[character_value]); 
        for (j = 0; j <= i - 1; j++) {
            sum = j + value;  
            lc.setColumn(addr, j, character[sum]);
        }  
        lc.setColumn(addr, j, 0x00);
        lc.setColumn(addr + 1, j, character[sum]);       
        delay(20);         
    }  
}
