#pragma once

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#if __has_include("ArduinoFake.h")
#include "ArduinoFake.h"
#else
#include <WProgram.h>
#endif
#endif

#include <LedController_byteblock.hpp>

#ifdef __has_include

#define INCLUDED_PGMSPACE
#if (__has_include(<avr/pgmspace.h>))
#include <avr/pgmspace.h>
#else
#if (__has_include(<pgmspace.h>))
#define INCLUDED_PGMSPACE
#include <pgmspace.h>
#else
#define INCLUDED_PGMSPACE
#ifndef pgm_read_byte
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_byte_near
#define pgm_read_byte_near(addr)  pgm_read_byte(addr)
#endif
#endif
#endif

#endif

#ifndef INCLUDED_PGMSPACE
#include <pgmspace.h>
#define INCLUDED_PGMSPACE
#endif

#include <SPI.h>

#include <LedController_config.hpp>

/**
 * \~
 * @todo make it threading safe
 * 
 * \~english
 * @brief This class provied a control interface for MAX7219 and MAX7221 Led
 * display drivers.
 * @details This Controller Class is mainly target at led matracies consisting
 * of more than 1 segment. While it can also handle 7-Segment Displays it is not
 * tested that well.
 * @warning This object is not thread safe yet.
 * 
 * \~german
 * @brief  Diese Klasse bietet ein Interface zur Steuerung von MAX7219 uns MAX7221 LED
 * Displaytreibern.
 * @details Diese Steuerundsklasse fokusiert sich primär auf Led Matrizen bestehend
 * aus mehr als einem Segment. Auch wenn sie 7-Segmentanzeigen steuern kann ist dies nicht getestet.
 * @warning Diese Klasses ist nicht therding-sicher.
 *
 */
template <size_t columns, size_t rows>
class LedController {
protected:

  /**
   * \~english
   * @brief This array contains the state of all of the LEDs.
   * 
   * \~german
   * @brief Dieses Array beinhaltet den Status von allen LEDs.
   * 
   */
  ByteBlock LedStates[columns*rows];

  /**
   * \~english
   * @brief The configuration of the LedController
   * 
   * \~german
   * @brief Die Konfiguration des LedController
   *
   */
  controller_configuration<columns,rows> conf;

  /**
   * \~english
   * @brief This function transfers one command to the attached module
   *
   * @param segment The segment that should execute this command
   * @param opcode The command that should be executed
   * @param data The data needed for that command
   * 
   * \~german
   * @brief Diese Funktion überträgt einen Befehl an das angeschlosssene Modul
   *
   * @param segment Das Segment, das den Befehl ausführen soll.
   * @param opcode Der Befehl, der ausgeführt werden soll.
   * @param data Die Daten, die für den Befehl benötigt werden.
   */
  void spiTransfer(unsigned int segment, byte opcode, byte data);
  
  /**
   * \~english
   * @brief The array for shifting the data to the devices
   * 
   * \~german
   * @brief Das Array was benötigt wird, um Daten auf das Gerät zu übertragen.
   * 
   */
  byte spidata[rows][columns * 2];

  /**
   * \~english
   * @brief Set the brightness of the segment.
   *
   * @param segmentNumber the address of the segment to control
   * @param newIntesityLevel the brightness of the display. (0..15)
   * 
   * \~german
   * @brief Setzte die Helligkeit eines Segments
   *
   * @param segmentNumber Die Nummer, des Segemnts.
   * @param newIntesityLevel Die gewünschte Helligkeit inklusive der Grenzen von 0 & 15. (0..15)
   */
  void setIntensity(unsigned int segmentNumber, unsigned int newIntesityLevel);

  /**
   * \~english
   * @brief True if the LedController is fully initilized
   *
   * \~german
   * @brief True wenn, der LedController vollständig initialisiert ist.
   *
   */
  bool initilized = false;

  /**
   * \~english
   * @brief initilize the internal buffers of the LedController.
   * 
   * \~german
   * @brief Initialisiere die internen Puffer des LedController.
   */
  void resetBuffers();

  /**
   * \~english
   * @brief initilize the spi outputs
   * 
   * \~german
   * @brief Initialisiere die SPI Ausgabe.
   */
  void initSPI();

  /**
   * \~english
   * @brief initilize the configuration
   * 
   * \~german
   * @brief Initialisiere die Konfiguration
   */
  void initConf();

public:
  /**
   * \~english
   * @brief Construct a new LedController without initilizing anything.
   *
   * \~german
   * @brief Erstelle einen neuen LedController ohne etwas zu initialisieren.
   */
  LedController();

  /**
   * \~english
   * @brief Construct a new LedController for use with hardware SPI
   * @note Only virtualMultiRow is possible with this Constructor.
   * A custom configuration is needed to have different CS Pins for each row.
   *
   * @param csPin The pin to select the led matrix
   * 
   * \~german
   * @brief Erstelle einen neuen LedController für die Nutzung mit hardwareSPI
   * @note Nur virtualMultiRow ist mit diesem Konstruktor möglich.
   * Eine angepasste Konfiguration ist notwendig, um untersciedliche CS Pins für jede Zeile zu haben.
   *
   * @param csPin Der CS Pin über den die Matrix gesteuert wird.
   */
  LedController(unsigned int csPin);

  /**
   * \~english
   * @brief Construct a new LedController object
   * @note Only virtualMultiRow is possible with this Constructor.
   * A custom configuration is needed to have different CS Pins for each row.
   *
   * @param dataPin pin on the Arduino where data gets shifted out (DIN)
   * @param clkPin pin for the clock (CLK)
   * @param csPin pin for selecting the device (CS)
   * @param useHardwareSpi true if you want to use hardware SPI (view
   * https://www.arduino.cc/en/Reference/SPI for pin config)
   * 
   * \~german
   * @brief Konstruiere ein neues LedController Objekt mit gegeben Pins.
   * @note Nur virtualMultiRow ist mit diesem Konstruktor möglich.
   * Eine angepasste Konfiguration ist notwendig, um untersciedliche CS Pins für jede Zeile zu haben.
   * 
   * @param dataPin Der Pin an dem die Daten ausgegeben werden (DIN)
   * @param clkPin Der Pin für das Taktsignal (CLK)
   * @param csPin Der Pin zum auswählen der Matrix (CS)
   * @param useHardwareSpi true wenn hardware SPI genutzt werden soll (schaue
   * https://www.arduino.cc/en/Reference/SPI für die Pin Konfiguration)
   */
  LedController(unsigned int dataPin, unsigned int clkPin, unsigned int csPin,
                bool useHardwareSpi = false);

  /**
   * \~english
   * @brief Construct a new Led Controller from a given configuration
   *
   * @param configuration the configuration that should be used for the
   * Controller
   * 
   * \~german
   * @brief Erstelle ein neues LedController Objekt aus einer Konfiguration.
   *
   * @param configuration die Konfiguration die zum erstellen verwendet werden soll.
   */
  LedController(const controller_configuration<columns,rows> &configuration);

  /**
   * \~english
   * @brief Destroy the Led Controller object and free the memory
   *
   * \~german
   * @brief Zerstöre das Objekt und leere den Speicher.
   */
  ~LedController();

  /**
   * \~english
   * @brief The copy constructor for the LedController
   *
   * @param other the LedController which should have its state copied
   * 
   * \~german
   * @brief Der Kopierkonstruktor für den LedController.
   *
   * @param other der LedController, der kopiert werden soll.
   */
  LedController(const LedController &other);

  /**
   * \~english
   * @brief initilizes the LedController for use with hardware SPI
   * @note Only virtualMultiRow is possible with this initilization.
   * A custom configuration is needed to have different CS Pins for each row.
   *
   * @param csPin The pin to select the led matrix
   * 
   * \~german
   * @brief Initialisiere den LedController für Nutzung mit hardware SPI
   * @note Nur virtualMultiRow ist mit diesem Initialisieren möglich.
   * Eine angepasste Konfiguration ist notwendig, um untersciedliche CS Pins für jede Zeile zu haben.
   *
   * @param csPin Der Pin um die LedMatrix auszuwählen.
   */
  void init(unsigned int csPin);

  /**
   * \~english
   * @brief initilizes the LedController
   * @note Only virtualMultiRow is possible with this initilization.
   * A custom configuration is needed to have different CS Pins for each row.
   *
   * @param dataPin pin on the Arduino where data gets shifted out (DIN)
   * @param clkPin pin for the clock (CLK)
   * @param csPin pin for selecting the device (CS)
   * @param useHardwareSpi true if you want to use hardware SPI (view
   * https://www.arduino.cc/en/Reference/SPI for pin config)
   * 
   * \~german
   * @brief Initialisiere einen LedController Objekt mit gegeben Pins.
   * @note Nur virtualMultiRow ist mit diesem Initialisieren möglich.
   * Eine angepasste Konfiguration ist notwendig, um untersciedliche CS Pins für jede Zeile zu haben.
   * 
   * @param dataPin Der Pin an dem die Daten ausgegeben werden (DIN)
   * @param clkPin Der Pin für das Taktsignal (CLK)
   * @param csPin Der Pin zum auswählen der Matrix (CS)
   * @param useHardwareSpi true wenn hardware SPI genutzt werden soll (schaue
   * https://www.arduino.cc/en/Reference/SPI für die Pin Konfiguration)
   */
  void init(unsigned int dataPin, unsigned int clkPin, unsigned int csPin,
            bool useHardwareSpi = false);

  /**
   * \~english
   * @brief initilizes the LedController with a given configuration
   *
   * @param configuration The configuration that should be used
   * 
   * \~german
   * @brief Initialisiere einen LedController mit einer gegebenen Konfiguration.
   *
   * @param configuration Die Konfiguration, die verwendet werden soll.
   */
  void init(const controller_configuration<columns,rows> &configuration);

  /**
   * \~english
   * @brief returns the status of the LedController
   *
   * @return true the LedController is initilized
   * @return false the LedController is not initilized
   * 
   * \~german
   * @brief gibt den Status des LedController zurück.
   *
   * @return true der LedController ist initialisiert
   * @return false der LedController ist nicht initialisiert
   */
  bool isInitilized();

  /**
   * \~english
   * @brief Set the Intensity of the whole matrix to the given value.
   * @note if you want to save more energy disable segments you don't need or
   * lower the brightness.
   * @param newIntesityLevel the new brightness of the matrix. (0..15)
   * 
   * \~german
   * @brief Setzt die Helligkeit der gesamten Matrix auf den gegebenen Wert.
   * @note Falls du Energie sparen willst, schalte segmente aus, 
   * die nicht benötigt werden oder verringere die Helligkeit.
   * @param newIntesityLevel the new brightness of the matrix. (0..15)
   */
  void setIntensity(unsigned int newIntesityLevel);

  /**
   * \~english
   * @brief Display 8 lines on the given segment
   *
   * @param segmentindex the Segment number of the desired segment
   * @param data an array containing the data for all the pixels that should be
   * displayed on that segment
   * 
   * \~german
   * @brief Zeige einen gegebenen ByteBlock auf einem Segemnt an (setzte alle 8 Zeilen).
   *
   * @param segmentindex Die Nummer des gewünschten Segments
   * @param data Die Daten, die angezeigt werden sollen.
   */
  void displayOnSegment(unsigned int segmentindex, ByteBlock data);

  /**
   * \~english
   * @brief  Display 8 lines on the given segment
   * 
   * @param column the column where the wanted segment is
   * @param row_num the row where the wanted segment is
   * @param data an array containing the data for all the pixels that should be
   * displayed on that segment
   * 
   * \~german
   * @brief Zeige einen gegebenen ByteBlock auf einem Segemnt an (setzte alle 8 Zeilen).
   *
   * @param column Die Spalte in der das Segment ist.
   * @param row_num Die Zeile in der das Segment ist.
   * @param data Die Daten, die angezeigt werden sollen.
   */
  void displayOnSegment(unsigned int column, unsigned int row_num, ByteBlock data);

  /**
   * \~english
   * @brief Get the Segment Data of a specific Segment
   * 
   * @param column the column where the wanted segment is
   * @param row_num the row where the wanted segment is
   * @return ByteBlock the requested segment
   * 
   * \~german
   * @brief Hole den Zustand eines Segments.
   *
   * @param column Die Spalte in der das Segment ist.
   * @param row_num Die Zeile in der das Segment ist.
   * @return ByteBlock Die Daten, die in dem Segment angezeigt werden.
   */
  ByteBlock getSegmentData(unsigned int column, unsigned int row_num);

  /**
   * \~english
   * @brief Get the Segment Data of a specific Segment
   * 
   * @param segmentindex the index of whose data you want to have
   * @return ByteBlock the requested segment
   * 
   * \~german
   * @brief Hole den Zustand eines Segments.
   *
   * @param segmentindex Die Nummer des gewünschten Segments
   * @return ByteBlock Die Daten, die in dem Segment angezeigt werden.
   */
  ByteBlock getSegmentData(unsigned int segmentindex);

  /**
   * \~english
   * @brief activates all segments, sets to same intensity and clears them
   *
   * \~german
   * @brief aktiviere alle Segmente, setzte alle auf die gleiche Helligkeit und leere den Inhalt.
   */
  void resetMatrix();

  /**
   * \~english
   * @brief clears all segments, turning all LEDs off.
   *
   * \~german
   * @brief leere alle Segmente (alle LEDs aus).
   */
  void clearMatrix();

  /**
   * \~english
   * @brief clears a given segment, turning all its LEDs off.
   *
   * @param segmentNumber The segment to control.
   * 
   * \~german
   * @brief leere ein Segment (alle LEDs aus).
   *
   * @param segmentNumber Die Nummer des gewünschten Segments.
   */
  void clearSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief Get the number of configured segments
   *
   * @return unsigned int The number of configured segments
   * 
   * \~german
   * @brief Hole die geasmate Anzahl der konfigurierten Segmenten.
   *
   * @return unsigned int Die Anzahl aller konfigurierten Segemente.
   */
  unsigned int getSegmentCount();

  /**
   * \~english
   * @brief Set the segment in power-down mode.
   *
   * @param segmentNumber The segment to control
   * 
   * \~german
   * @brief Deaktiviere ein Segment.
   *
   * @param segmentNumber Die Nummer des gewünschten Segments.
   */
  void shutdownSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief Get the segment out of power-down mode for normal operation.
   *
   * @param segmentNumber The segment to control
   * 
   * \~german
   * @brief Aktiviere ein Segment.
   *
   * @param segmentNumber Die Nummer des gewünschten Segments.
   */
  void activateSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief Set all segments into power-down mode
   *
   * \~german
   * @brief Deaktiviere alle Segmente.
   */
  void shutdownAllSegments();

  /**
   * \~english
   * @brief Get all segments out of power-down mode for normal operation.
   *
   * \~german
   * @brief Aktiviere alle Segmente.
   */
  void activateAllSegments();

  /**
   * \~english
   * @brief Set the number of digits (or rows) to be displayed (default: 7).
   * @warning See datasheet for sideeffects of the scanlimit on the brightness of
   * the display.
   * @param segmentNumber The segment which should be addressed
   * @param limit The number of digits to be displayed (0..7)
   * 
   * \~german
   * @brief Setzte die Anzahl der Zeilen/Stellen, die auf einem Segment angezeigt werden.
   * @warning Schau in das Datenblatt für Nebenwirkungen vom scanlimit auf die Helligkeit.
   * @param segmentNumber Die nummer des gewünschten Segments.
   * @param limit Die Anzahl der Zeilen/Stellen die angezwigt werden sollen (0..7)
   */
  void setScanLimit(unsigned int segmentNumber, unsigned int limit);

  /**
   * \~english
   * @brief Set one Row of one segment.
   *
   * @param segmentNumber The Segment which should be modified
   * @param row The row which should be modified
   * @param value each bit set to 1 will light up the corresponding Led.
   * 
   * \~german
   * @brief Setze eine Reihe eines Segments.
   *
   * @param segmentNumber Die Nummer des gewünschten Segments
   * @param row Die Reihe in dem Segment, welche gesetzte werden soll.
   * @param value Der Wert der gesetzt werden soll.
   */
  void setRow(unsigned int segmentNumber, unsigned int row, byte value);

  /**
   * \~english
   * @brief get one Row of one segment.
   *
   * @param segmentNumber The Segment which should be modified
   * @param row The row which should be modified
   * @return value each bit set to 1 will light up the corresponding Led.
   * 
   * \~german
   * @brief Hole den Zustand einer Reihe eines Segments.
   *
   * @param segmentNumber Die Nummer des gewünschten Segments
   * @param row Die Reihe in dem Segment, welche gewünscht ist.
   * @return byte Der Wert, der angezeigt wird.
   */
  byte getRow(unsigned int segmentNumber, unsigned int row);

  /**
   * \~english
   * @brief Set a single led to a given value
   *
   * @param segmentNumber the segment number of the desired led
   * @param row the row of the desired led (0..7)
   * @param column the column of the desired led (0..7)
   * @param state true if it should be on otherwise false
   * 
   * \~german
   * @brief Setzte eine einzalne Led auf einen bestimmten Wert
   *
   * @param segmentNumber Die Nummer des gewünschten Segments
   * @param row die Zeile der LED im Segment (0..7)
   * @param column die Spalte der LED im Segment (0..7)
   * @param state true wenn sie an sein soll, ansonsten false
   */
  void setLed(unsigned int segmentNumber, unsigned int row, unsigned int column,
              boolean state);

  /**
   * \~english
   * @brief Set one column of a given segment
   *
   * @param segmentNumber The desired Segment number
   * @param col The desired column
   * @param value The value, this column should have
   * 
   * \~german
   * @brief Setzte eine Spalte eines Segments
   *
   * @param segmentNumber Die Nummer des gewünschten Segments
   * @param col  die Spalte im Segment
   * @param value Der Wert der gesetzt werden soll.
   */
  void setColumn(unsigned int segmentNumber, unsigned int col, byte value);

  /**
   * \~english
   * @brief Set a hexadecimal digit on a 7-Segment Display
   *
   * @param segmentNumber The number of the desired Segment
   * @param digit the position of the digit on the Segment (0..7)
   * @param value the value to be displayed. (0x00..0x0F)
   * @param dp if true sets the decimal point
   * 
   * \~german
   * @brief Setzte eine Hexadecimalziffer auf einem 7-Segment Display
   *
   * @param segmentNumber Die Nummer des gewünschten Segments
   * @param digit Die Position der Ziffer (0..7)
   * @param value Der Wert, der angezeigt werden soll. (0x00..0x0F)
   * @param dp true wenn der Dezimalpunkt gesetzt werden soll.
   */
  void setDigit(unsigned int segmentNumber, unsigned int digit, byte value,
                boolean dp);

  /**
   * \~english
   * @brief Set the Display a character on a 7-Segment display.
   * @note There are only a few characters that make sense here :
   *	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
   *  'A', 'b', 'c', 'd', 'E', 'F', 'H', 'L', 'P',
   *  '.', '-', '_', ' '
   * @param segmentNumber The number of the desired segment
   * @param digit the position of the character on the segment (0..7)
   * @param value the character to be displayed.
   * @param dp dp if true sets the decimal point
   * 
   * \~german
   * @brief Setzte ein Zeichen auf einem 7-Segment display.
   * @note Es gibt nur einige Zeichen die Sinn ergeben:
   *	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
   *  'A', 'b', 'c', 'd', 'E', 'F', 'H', 'L', 'P',
   *  '.', '-', '_', ' '
   * @param segmentNumber Die Nummer des gewünschten Segments
   * @param digit Die Position der Zeichens (0..7)
   * @param value Der Wert, der angezeigt werden soll.
   * @param dp true wenn der Dezimalpunkt gesetzt werden soll.
   */
  void setChar(unsigned int segmentNumber, unsigned int digit, char value,
               boolean dp);

  /**
   * \~english
   * @brief refreshes all segments by first resetting them and then updating
   * them.
   *
   * \~german
   * @brief Lädt alle Segmente neu (zurücksetzten und dann updaten).
   */
  void refreshSegments();

  /**
   * \~english
   * @brief refreshes a given segment by first resetting it and then updating
   * it.
   *
   * @param segmentNumber the segment that will be resetted
   * 
   * \~german
   * @brief Lädt eine Segment neu (zurücksetzten und dann updaten).
   *
   * @param segmentNumber Die Nummer des gewünschten Segments
   */
  void refreshSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief update all segments by displaying the internally stored state of the
   * segments.
   *
   * \~german
   * @brief Lädt den Inhalt der internen Daten auf alle Segmente hoch.
   */
  void updateSegments();

  /**
   * \~english
   * @brief update a given segment by displaying the internally stored state of
   * the segment.
   *
   * @param segmentNumber the segment that will be updated
   * 
   * \~german
   * @brief Lädt den Inhalt der internen Daten auf ein Segmente hoch.
   *
   * @param segmentNumber Die Nummer des gewünschten Segments
   */
  void updateSegment(unsigned int segmentNumber);

  /**
   * \~english
   * @brief moves the data left by one
   *
   * @param shiftedInColumn The column that will be shifted to the right
   * (default 0x00)
   * @param row_num The row that will be shifted to the right
   * @warning ONLY moves one row
   * @return byte The column that gets shifted out on the left
   * 
   * \~german
   * @brief schiebt die Daten einer Zeile eins nach links.
   *
   * @param shiftedInColumn Die Daten, die rechts reingeschoben werden
   * (default 0x00)
   * @param row_num Die Zeile, die geschoben werden soll
   * @warning Nur EINE ZEILE wird verschoben
   * @return byte Die Daten, die links rausgeschoben werden.
   */
  byte moveRowLeft(byte shiftedInColumn = 0x00, unsigned int row_num = 0);

  /**
   * \~english
   * @brief moves the data left by one
   *
   * @param shiftedInColumn The column that will be shifted to the left
   * (default 0x00)
   * @param row_num The row that will be shifted to the left
   * @warning ONLY moves one row
   * @return byte The column that gets shifted out on the right
   * 
   * \~german
   * @brief schiebt die Daten einer Zeile eins nach rechts.
   *
   * @param shiftedInColumn Die Daten, die links reingeschoben werden
   * (default 0x00)
   * @param row_num Die Zeile, die geschoben werden soll
   * @warning Nur EINE ZEILE wird verschoben
   * @return byte Die Daten, die rechts rausgeschoben werden.
   */
  byte moveRowRight(byte shiftedInColumn = 0x00, unsigned int row_num = 0);

  /**
   * \~english
   * @brief moves the data of a column up by one
   * 
   * @param shiftedInRow the date the will be shifted in on the bottom
   * @param col_num the index of the column that will be moved
   * @return byte the row that will be shifted out
   * 
   * \~german
   * @brief schiebt die Daten einer Spalte eins nach oben.
   *
   * @param shiftedInColumn Die Daten, die unten reingeschoben werden
   * (default 0x00)
   * @param row_num Die Spalte, die geschoben werden soll
   * @warning Nur EINE SPALTE wird verschoben
   * @return byte Die Daten, die oben rausgeschoben werden.
   */
  byte moveColumnUp(byte shiftedInRow = 0x00, unsigned int col_num = 0);

  /**
   * \~english
   * @brief moves the data of a column down by one
   * 
   * @param shiftedInRow the date the will be shifted in on the top
   * @param col_num the index of the column that will be moved
   * @return byte the row that will be shifted out
   * 
   * \~german
   * @brief schiebt die Daten einer Spalte eins nach unten.
   *
   * @param shiftedInColumn Die Daten, die oben reingeschoben werden
   * (default 0x00)
   * @param row_num Die Spalte, die geschoben werden soll
   * @warning Nur EINE SPALTE wird verschoben
   * @return byte Die Daten, die unten rausgeschoben werden.
   */
  byte moveColumnDown(byte shiftedInRow = 0x00, unsigned int col_num = 0);

  /**
   * \~english
   * @brief moves the data up by one and 0x00 will be shifted in
   *
   * \~german
   * @brief Schibet die Daten eins nach oben und 0x00 wird unten reingeschoben.
   */
  void moveUp();

  /**
   * \~english
   * @brief moves the data down by one and 0x00 will be shifted in
   *
   * \~german
   * @brief Schibet die Daten eins nach unten und 0x00 wird oben reingeschoben.
   */
  void moveDown();

  /**
   * \~english
   * @brief moves the data left by one and 0x00 will be shifted in
   *
   * \~german
   * @brief Schibet die Daten eins nach links und 0x00 wird recht reingeschoben.
   */
  void moveLeft();

  /**
   * \~english
   * @brief moves the data right by one and 0x00 will be shifted in
   *
   * \~german
   * @brief Schibet die Daten eins nach rechts und 0x00 wird links reingeschoben.
   */
  void moveRight();

  /**
   * \~english
   * @brief moves all rows to the left.
   * 
   * @param shiftedInColumn This Array contains what will be shifted in on each Row.
   * @return ByteRow<rows> This Array contains the bytes that will be shifted out on each Row.
   * 
   * \~german
   * @brief Schiebt alle Reihen eins nach links
   * 
   * @param shiftedInColumn Die Splaten die reingeschoben werden.
   * @return ByteRow<rows> Die Splaten die rausgeschoben werden.
   */
  ByteRow<rows> moveLeft(const ByteRow<rows>& shiftedInColumn);

  /**
   * \~english
   * @brief moves all rows to the right.
   * 
   * @param shiftedInColumn This Array contains what will be shifted in on each Row.
   * @return ByteRow<rows> This Array contains the bytes that will be shifted out on each Row.
   * 
   * \~german
   * @brief Schiebt alle Reihen eins nach rechts
   * 
   * @param shiftedInColumn Die Splaten die reingeschoben werden.
   * @return ByteRow<rows> Die Splaten die rausgeschoben werden.
   */
  ByteRow<rows> moveRight(const ByteRow<rows>& shiftedInColumn);

  /**
   * \~english
   * @brief moves all columns up.
   * 
   * @param shiftedInColumn This Array contains what will be shifted in on each Row
   * @return ByteRow<columns> This Array contains the bytes that will be shifted out on each Row.
   * 
   * \~german
   * @brief Schiebt alle Splaten eins nach oben
   * 
   * @param shiftedInColumn Die Reihen die reingeschoben werden.
   * @return ByteRow<rows> Die Reihen die rausgeschoben werden.
   */
  ByteRow<columns> moveUp(const ByteRow<columns>& shiftedInColumn);

  /**
   * \~english
   * @brief moves all columns down.
   * 
   * @param shiftedInColumn This Array contains what will be shifted in on each Row.
   * @return ByteRow<columns> This Array contains the bytes that will be shifted out on each Row.
   * 
   * \~german
   * @brief Schiebt alle Splaten eins nach unten
   * 
   * @param shiftedInColumn Die Reihen die reingeschoben werden.
   * @return ByteRow<rows> Die Reihen die rausgeschoben werden.
   */
  ByteRow<columns> moveDown(const ByteRow<columns>& shiftedInColumn);

  /**
   * \~english
   * @brief This function changes to bitorder of a byte (useful to mirror
   * "images" you want to display)
   *
   * @param input The byte that should be reversed
   * @return byte The reversed byte
   * 
   * \~german
   * @brief Diese Funktion ändert die Bitreihnfolge eines Bytes bzw. kehrt ein Bit um.
   *
   * @param input Das byte, das umgekehrt werden soll.
   * @return byte Das umgekehrte Byte
   */
  byte reverse(byte input);

  /**
   * \~english
   * @brief Reverse an ByteBlock of 8 bytes (mirror it)
   * 
   * @param input The ByteBlock that should be mirrored
   * @return ByteBlock The reversed ByteBlock
   * 
   * \~german
   * @brief Diese Funktion kehrt die Reihnfolge der einzelnen Zeilen um (spiegeln in x-Richtung).
   * 
   * @param input Der ByteBlock der umgekehrt werden soll.
   * @return ByteBlock Der umgekehrte ByteBlock.
   */
  ByteBlock reverse(ByteBlock input);

  /**
   * \~english
   * @brief Turns an ByteBlock of rows into an ByteBlock of columns
   * 
   * @param rowArray the ByteBlock of rows of which you want the columns of
   * @return ByteBlock The Columns of the given ByteBlock
   * 
   * \~german
   * @brief Wandelt einen Block aus Zeilen in einen Block aus Spalten.
   * 
   * @param rowArray Der Block aus Zeilen
   * @return ByteBlock Die Splaten des gegebenen Blocks
   */
  ByteBlock makeColumns(ByteBlock rowArray);

  /**
   * \~english
   * @brief rotate an ByteBlock by 180 degrees
   * 
   * @param input the ByteBlock that will be rotated
   * @return ByteBlock The rotated ByteBlock
   * 
   * \~german
   * @brief rotiert einen ByteBlock um 180 Grad
   * 
   * @param input Der zu rotierende ByteBlock.
   * @return ByteBlock Der rotierte byteBlock.
   */
  ByteBlock rotate180(ByteBlock input);

  /**
   * \~english
   * @brief Get the Config of the Led Controller
   *
   * @return controlller_configuration the configuration
   * 
   * \~german
   * @brief Hole die Konfiguration des LedController
   *
   * @return controlller_configuration Die aktuelle Konfiguration
   */
  const controller_configuration<columns,rows>& getConfig();

  //The following methods are deprecated and will be removed in the future
  //They only exist to help the transition to a new version
  //Because they will be removed in the future the documentation will not be translated

  /**
   * \~
   * @deprecated to be reomoved in version 2.1.0, moveRowLeft should be used
   * @todo remove function in version 2.1.0
   * 
   * \~english
   * @brief moves the data left by one
   * @param shiftedInColumn The column that will be shifted to the right
   * (default 0x00)
   * @warning ONLY moves row 0, this function exists for backwards compatibility
   * @return byte The column that gets shifted out on the left
   */
  byte moveLeft(byte shiftedInColumn);

  /**
   * \~
   * @deprecated to be reomoved in version 2.1.0, moveRowLeft should be used
   * @todo remove function in version 2.1.0
   * 
   * \~english
   * @brief moves the data left by one
   * @param shiftedInColumn The column that will be shifted to the left
   * (default 0x00)
   * @warning ONLY moves row 0, this function exists for backwards compatibility
   * @return byte The column that gets shifted out on the right
   */
  byte moveRight(byte shiftedInColumn);

  /**
   * \~
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief Get the Segment Data of a specific Segment
   * @param column the column where the wanted segment is
   * @param row_num the row where the wanted segment is
   * @param resultLocation the location where the data should be stored
   */
  void getSegmentData(unsigned int column, unsigned int row_num, ByteBlock* resultLocation);

  /**
   * \~
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief Get the Segment Data of a specific Segment
   * @param segmentindex the index of whose data you want to have
   * @param resultLocation the location where the data should be stored
   */
  void getSegmentData(unsigned int segmentindex, ByteBlock *resultLocation);

  /**
   * \~
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief Turns an array of rows into an array of columns
   * @param rowArray the array of rows of which you want the columns
   * @param columnArray The address where the result will be stored
   */
  void makeColumns(ByteBlock rowArray, ByteBlock *columnArray);

  /**
   * \~
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief Reverse an array of 8 bytes (mirror it)
   * @param input The array that should be mirrored
   * @param reversedInput The address where the result will be stored
   */
  void reverse(ByteBlock input, ByteBlock *reversedInput);

  /**
   * \~
   * @deprecated the function with ByteBlock as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief rotate an byte[8] array by 180 degrees
   * @param input the array that will be rotated
   * @param rotatedInput The address where the result will be stored
   */
  void rotate180(ByteBlock input, ByteBlock *rotatedInput);

  /**
   * \~
   * @deprecated the function with ByteRow as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief moves the data up by one
   * @param shiftedInRow The row that will be shifted in on the bottom (default
   * 0x00)
   * @param shiftedOutRow The address of the row that will be shifted out on the
   * bottom
   */
  void moveUp(const ByteRow<columns>& shiftedInRow, ByteRow<columns>* shiftedOutRow);

  /**
   * \~
   * @deprecated the function with ByteRow as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief moves the data down by one
   * @param shiftedInRow The row that will be shifted in on the top (default
   * 0x00)
   * @param shiftedOutRow The address of the row that will be shifted out on the
   * bottom
   */
  void moveDown(const ByteRow<columns>& shiftedInRow, ByteRow<columns>* shiftedOutRow);

  /**
   * \~
   * @deprecated the function with ByteRow as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief moves the data up by oneand 0x00 will be shifted in
   * @param shiftedOutRow The address of the row that will be shifted out on the
   * bottom
   */
  void moveUp(ByteRow<columns>* shiftedOutRow);

  /**
   * \~
   * @deprecated the function with ByteRow as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief moves the data down by one and 0x00 will be shifted in
   * @param shiftedOutRow The address of the row that will be shifted out on the
   * bottom
   */
  void moveDown(ByteRow<columns>* shiftedOutRow);

  /**
   * \~
   * @deprecated the function with ByteRow as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief moves all rows to the left.
   * The passed Arrays need to have the same length as the number of rows, or be a nullptr.
   * If shiftedInColumn is a nullptr, 0x00 will be used for all rows.
   * @param shiftedInColumn This Array contains what will be shifted in on each Row and needs to be the same size as number of rows or nullptr.
   * @param shiftedOutColumn This pointer to an Array will contain the bytes that will be shifted out on each Row, it should be the same size as the number of rows or nullptr.
   */
  void moveLeft(const ByteRow<rows>& shiftedInColumn, ByteRow<rows>* shiftedOutColumn);

  /**
   * \~
   * @deprecated the function with ByteRow as return type should be used. Will be removed in version 2.2.0
   * @todo remove function in version 2.2.0
   * 
   * \~english
   * @brief moves all rows to the right.
   * The passed Arrays need to have the same length as the number of rows, or be a nullptr.
   * If shiftedInColumn is a nullptr, 0x00 will be used for all rows.
   * @param shiftedInColumn This Array contains what will be shifted in on each Row and needs to be the same size as number of rows or nullptr.
   * @param shiftedOutColumn This pointer to an Array will contain the bytes that will be shifted out on each Row, it should be the same size as the number of rows or nullptr.
   */
  void moveRight(const ByteRow<rows>& shiftedInColumn, ByteRow<rows>* shiftedOutColumn);
};

/**
 * @example LedControllerDemoRocketMulti.ino
 * 
 * \~english
 * An example on how to use the LedController with multiple rows
 * 
 * \~german
 * Ein Beispiel wie man den LedController mit mehreren Reihen nutzen kann
 */

/**
 * @example LedControllerDemoRocket.ino
 * 
 * \~english
 * An example on how to use the LedController to move a rocket across a led matrix
 * 
 * \~german
 * Ein Beispiel wie man den LedController nutzen kann um einen Rakete über die Led Matrix zu bewegen.
 */

/**
 * @example LedControllerDemoHwSPI.ino
 * 
 * \~english
 * This is simply @ref LedControllerDemoRocket.ino just with hardware spi.
 * 
 * \~german
 * Dies ist einfach @ref LedControllerDemoRocket.ino nur mit hardware spi.
 */

/**
 * @example LedControllerDemoCounting.ino
 * 
 * \~english
 * An example on how to use the LedController to create a counter.
 * 
 * \~german
 * Ein Beispiel wie man den LedController nutzen kann um einen Zähler zu erstellen.
 */

/**
 * @example LCDemo7Segment.ino
 * 
 * \~english
 * An example on how to use the LedController with 7-Segment displays.
 * 
 * \~english
 * Ein Beispiel wie man den LedController nutzen kann um 7-Segment Anzeigen zu steuern.
 */
