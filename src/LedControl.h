/*
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LedControl_h
#define LedControl_h

#include <avr/pgmspace.h>

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/*
 * Segments to be switched on for characters and digits on
 * 7-Segment Displays
 */
 
/* The following table is used to translate an ascii byte value 
 * to the corresponding 7segment bitmap pattern with segmentmapping "pabcdefg"
 * To give the easyest usability with the most possible response, lower or upper case versions of
 * letters are replaced by the other size, when the original size could not be designed properly. 
 * (Check the Comments to see the replacements)
 * Also some designs are ambigous (e.g. 2 and Z, 0 and O, ( and C ). 
 * This must be sovled by giving good context, choosing good words. 
 * which lies in the responsibility of the final design.
 * Some other designs are more a possibility, then a real representation, but may be more useful then just blank (e.g. :<=>)
 */
const static byte charTable [] PROGMEM  = {
//      0/8      1/9       2/A       3/B         4/C       5/D       6/E       7/F  
    B01111110,B00110000,B01101101,B01111001,  B00110011,B01011011,B01011111,B01110000, //  (0) 0123 4567
    B01111111,B01111011,B01110111,B00011111,  B00001101,B00111101,B01001111,B01000111, //  (8) 89AB CDEF
	B01100000,B00011000,B00001100,B01000010,  B01101100,B01011010,B00110001,B00000111, // (16) •••• ••••
    B01000000,B01001000,B01001001,B00000000,  B00000000,B00000000,B00000000,B00000000, // (24) •••• ••••
    B00000000,B00000000,B00100010,B00000000,  B00000000,B00000000,B00000000,B00100000, // (32)  ×"× ×××'   
    B01001110,B01111000,B01100011,B00000000,  B10000000,B00000001,B10000000,B00100100, // (40) ()*× ,-./ 
    B01111110,B00110000,B01101101,B01111001,  B00110011,B01011011,B01011111,B01110000, // (48) 0123 4567
    B01111111,B01111011,B00001001,B00000000,  B00000000,B01000011,B01000001,B01100001, // (56) 89:× ×<=>
    B00000000,B01110111,B01111111,B01001110,  B00111101,B01001111,B01000111,B01011110, // (64) ×ABC DEFG
    B00110111,B00000110,B00111100,B00000000,  B00001110,B00000000,B00010101,B01111110, // (72) HIJ× L×nO
    B01100111,B00000000,B00000000,B01011011,  B00001111,B00111110,B00000000,B00000000, // (80) P××S tU××
    B00000000,B00111011,B01101101,B00000000,  B00000000,B00010010,B01100010,B00001000, // (88) ×YZ× ×\^_
    B00000010,B01110111,B00011111,B00001101,  B00111101,B01001111,B01000111,B01011110, // (96) `Abc dEFG
    B00010111,B00010000,B00111100,B00000000,  B00110000,B00000000,B00010101,B00011101, //(104) hiJ× l×no
    B01100111,B00000000,B00000101,B01011011,  B00001111,B00011100,B00000000,B00000000, //(112) P×rS tu×× 
    B00000000,B00111011,B01101101,B00000000,  B00000110,B00000000,B00100101,B00000000  //(120) ×YZ× |×~×
};

class LedControl {
    private :
        /* The array for shifting the data to the devices */
        byte spidata[16];
        /* Send out a single command to the device */
        void spiTransfer(int addr, byte opcode, byte data);

        /* led-status for 8 devices with 8 elements containing 8 leds (7 segments + DP)  */
        byte status[64]; 
		/* bit flag, if display of device is rotated by 180 degrees */
		byte rotate180=0;
		
        /* Data is shifted out of this pin*/
        byte SPI_MOSI;
        /* The clock is signaled on this pin */
        byte SPI_CLK;
        /* This one is driven LOW for chip selectzion */
        byte SPI_CS;
        /* The maximum number of 7 segment devices we use */
        byte maxDevices;

    public:
        /* 
         * Create a new controler 
         * Params :
         * dataPin		pin on the Arduino where data gets shifted out
         * clockPin		pin for the clock
         * csPin		pin for selecting the device 
         * numDevices	maximum number of devices that can be controled
		 * rotate180Flags Flag for every device if it is mounted rotated by 180 degrees
         */
        LedControl(int dataPin, int clkPin, int csPin, int numDevices=1, uint8_t rotate180Flags=0);

        /*
         * Gets the number of devices attached to this LedControl.
         * Returns :
         * int	the number of devices on this LedControl (1..n)
         */
        int getDeviceCount();

        /* 
         * Set the shutdown (power saving) mode for the device
         * Params :
         * addr	The address of the display to control
         * status	If true the device goes into power-down mode. Set to false
         *		for normal operation.
         */
        void shutdown(int addr, bool status);

        /* 
         * Set the number of digits (or rows) to be displayed.
         * See datasheet for sideeffects of the scanlimit on the brightness
         * of the display.
         * Params :
         * addr	address of the display to control (0..n-1)
         * limit	number of digits to be displayed (1..8)
         */
        void setScanLimit(int addr, int limit);

        /* 
         * Set the brightness of the display.
         * Params:
         * addr		the address of the display to control (0..n-1)
         * intensity	the brightness of the display. (0..15)
         */
        void setIntensity(int addr, int intensity);

        /* 
         * Switch all Leds on the display off. 
         * Params:
         * addr	address of the display to control
         */
        void clearDisplay(int addr);

        /* 
         * Set the status of a single Led.
         * Params :
         * addr	address of the display  (0..n-1)
         * row	the row of the Led (0..7)
         * col	the column of the Led (0..7)
         * state	If true the led is switched on, 
         *		if false it is switched off
         */
        void setLed(int addr, int row, int col, boolean state);

        /* 
         * Set all 8 Led's in a row to a new state
         * Params:
         * addr	address of the display (0..n-1)
         * row	row which is to be set (0..7)
         * value	each bit set to 1 will light up the
         *		corresponding Led.
         */
        void setRow(int addr, int row, byte value);

        /* 
         * Set all 8 Led's in a column to a new state
         * Params:
         * addr	address of the display (0..n-1)
         * col	column which is to be set (0..7)
         * value	each bit set to 1 will light up the
         *		corresponding Led.
         */
        void setColumn(int addr, int col, byte value);

        /* 
         * Display a hexadecimal digit on a 7-Segment Display
         * Params:
         * addr	address of the display (0..n-1)
         * digit	the position of the digit on the display (0..7)
         * value	the value to be displayed. (0x00..0x0F)
         * dp	sets the decimal point.
         */
        void setDigit(int addr, int digit, byte value, boolean dp);

        /* 
         * Display a character (according to ascii codepage) on a 7-Segment display.
         * For the available characters check the char table
		 * in the upper part of this header file.

		 * Params:
         * addr	address of the display (0..n-1)
         * digit	the position of the character on the display (0..7)
         * value	the character to be displayed. 
         * dp	sets the decimal point.
         */
        void setChar(int addr, int digit, char value, boolean dp);

        /* 
         * Display a string  on a 7-Segment display. Starting at th defined the position
		 * Progressing left to right and ending with the last character. 
		 * Digits after the string will not be changed)
         * For the available characters check the char table
		 * in the upper part of this header file.
		 * Params:
         * addr	address of the display the string starts on (0..n-1)
         * digit	the start position of the string on the display (0..7)
         * theString	the character to be displayed. 
         * dotpattern	Bitpattern to set the dots additionally along the string starting with the leftmost bit
         */		
		void setString(int addr, int startDigit, String theString, uint8_t dotpattern);

        /* 
         * Define if the character font and digit adress of a deviceare in  calculated normal
		 * or for  180 dregee rotated display(Decimal points up)

		 * Params:
         * addr	address of the display (0..n-1)
         * isRotated	boolean to set the rotation of the display

         */
        void setRotate180(int addr, boolean isRotated);
		

};

#endif	//LedControl.h



