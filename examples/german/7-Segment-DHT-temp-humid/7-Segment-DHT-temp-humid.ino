// DTH Temperatur- und Luftfeuchtigkeitssensor auf einem 7-Segment Display angezeigt.

// Die folgenden Arduino Bibliotheken werden benötigt:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
// - LedController: https://github.com/noah1510/LedController

//Dieser Code basiert auf einem Beispiel der DHT Sensor Library.
//All der sensor bezogene Code wurde in Funktionen verpackt, um die integration des LedController besser zu zeigen.

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 17     // Digital pin vebunden mit dem DHT sensor 

// verwendeten Sensortyp auskommentieren:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// Anleitung für Verkabelung und Detials über die Sensoren (auf Englisch):
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

//Wie immer die Bibliothek einbinden
#include "LedController.hpp"

//Die Variable in der Controller gespeichert ist.
//Der Controller hat zwei Zeilen mit je einer Spalte.
//Bei 7-Segment-Displays hat jedes Segment 8 Ziffern, die angezeigt werden können.
LedController<1,2> control = LedController<1,2>();

//Die CS Pins für die einzelnen Zeilen
#define CS_BOTTOM 16
#define CS_TOP 22

//Die Anzahl der Ziffern, die für die Darstellung von floats verwendet werden soll
const unsigned int NUMBER_OF_DIGITS = 4;

//Wie viele Werte zur Glättung gespeichert werden sollen.
#define SMOOTHING_BUFFER_SIZE 10

//Diese Funktion gibt die Rohdaten vom Sensor zurück.
//Die Einheit ist Grad Celsius.
//Weil keine Glättung stattfindet, kann der Wert stark schwanken.
//Falls es einen Fehler beim Auslesen gab, wird ein Wert zurückgegeben der niedriger ist als der absolute Nullpunk (-273.15).
float getRawTemp(){
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    return -1000;
  }

  return event.temperature;
}

//Diese Funktion gibt die Rohdaten vom Sensor zurück.
//Die relative Luftfeuchtigkeit wird zurückgegeben, folglich sind die Werte zwischen 0 und 100.
//Weil keine Glättung stattfindet, kann der Wert stark schwanken.
//Falls es einen Fehler beim Auslesen gab, wird ein Wert zurückgegeben der negativ ist.
float getRawHumid(){
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    return -1;
  }
  return event.relative_humidity;
}

//Diese Funktion gibt geglättete Werte für fie Temperatur zurück.
//Die Einheit ist Grad Celsius.
//Falls man schnelle Änderungen erwartet, sollte man die Rohdaten verwenden.
//Der Rückgabewert ist immer gültig, aber erst gut nach SMOOTHING_BUFFER_SIZE aufrufen.
float getTemp(){
  static float last_temps[SMOOTHING_BUFFER_SIZE];
  static bool firstCall = true;
  
  float new_temp = getRawTemp();
  float sum = new_temp;
  
  if(new_temp < -370.0){
    sum = 0;
    for(unsigned int i = 0; i < SMOOTHING_BUFFER_SIZE;i++){
      sum += last_temps[i];
    }
  }else{
    for(unsigned int i = 0; i < SMOOTHING_BUFFER_SIZE-1;i++){
      last_temps[i] = firstCall ? new_temp : last_temps[i+1];
      sum += last_temps[i];
    }
    last_temps[SMOOTHING_BUFFER_SIZE-1] = new_temp;

    if(firstCall){firstCall = false;};
  }
  
  return sum/SMOOTHING_BUFFER_SIZE;
}

//Diese Funktion gibt geglättete Werte für fie Luftfeuchtigkeit zurück.
//Die relative Luftfeuchtigkeit wird zurückgegeben, folglich sind die Werte zwischen 0 und 100.
//Falls man schnelle Änderungen erwartet, sollte man die Rohdaten verwenden.
//Der Rückgabewert ist immer gültig, aber erst gut nach SMOOTHING_BUFFER_SIZE aufrufen.
float getHumidity(){
  static float last_humids[SMOOTHING_BUFFER_SIZE];
  static bool firstCall = true;
  
  float new_humid = getRawHumid();
  float sum = new_humid;
  
  if(new_humid < 0){
    sum = 0;
    for(unsigned int i = 0; i < SMOOTHING_BUFFER_SIZE;i++){
      sum += last_humids[i];
    }
  }else{
    
    for(unsigned int i = 0; i < SMOOTHING_BUFFER_SIZE-1;i++){
      last_humids[i] = firstCall ? new_humid : last_humids[i+1];
      sum += last_humids[i];
    }
    last_humids[SMOOTHING_BUFFER_SIZE-1] = new_humid;

    if(firstCall){firstCall = false;};
  }

  return sum/SMOOTHING_BUFFER_SIZE;
}

//Diese Funktion kann einen float Wert auf einer 7-Segment-anzeige anzeigen.
//Es wird die Annahme getroffen, dass der LedController nur eine Spalte hat.
//Die gewünschte Zeile kann mit den zweiten Parameter ausgewählt werden.
//Das dritte Parameter bestimmt die Anzahl der Stellen nach dem Komma.
//Das vierte Parameter schiebt die angezeigte Zahl nach links.
void displayFloat(float value, unsigned int row = 0, unsigned int decimalPlaces = 1,unsigned int digitOffset = 0){
  unsigned int total_length = NUMBER_OF_DIGITS;
  if(NUMBER_OF_DIGITS<decimalPlaces){return;};

  if(value < 0){
    control.setChar(row,total_length-1+digitOffset,'-',false);
    total_length--;
  };

  for(unsigned int i = 0; i < decimalPlaces; i ++){
    value*=10.0f;
  }

  unsigned int v = (unsigned int) (value < 0 ? -value : value);

  for (unsigned int i = 0; i < total_length;i++){
    control.setDigit(row,i+digitOffset,v%10,i == decimalPlaces);
    v/=10;
  }

}

//Eine einfache Ausgabe der Sensorinformationen über das Serial interface.
void printSensorInfo(){
  sensor_t sensor;

  //Temeraturdetails:
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));

  //Luftfeiuchtigkeitdetails
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
}

void setup() {
  //Initialisieren der Serial interface.
  Serial.begin(19200);
  //initialisieren des Temperatur/Luftfeuchtigkeitssensor.
  dht.begin();
  
  //Setzt die Verzögerung zwischen zwei Messungen basierend auf den Sensorinformationen
  sensor_t sensor;
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  //Konfiguration für den Controller
  controller_configuration<1,2> conf;

  //deaktivieren von virtual_muti_row
  conf.SPI_CS = 0;
  conf.virtual_multi_row = false;

  //hier werden die Chip Select Pins der einzelnen Reihen gesetzt.
  //Die untere Zeile (Zeile 0) ist an Pin 16 geschlossen und die obere Reihe (Reihe 1) an Pin 22
  conf.row_SPI_CS[0] = CS_BOTTOM;
  conf.row_SPI_CS[1] = CS_TOP;

  //Das setzt die nńutzung von harware SPI
  //Schaue welche Pins bei deinem Board verwendet werden müssen.
  conf.useHardwareSpi = true;

  //Das schaltet die Debug Ausgabe an.
  //Es sollte zwar nichts ausgegeben werden, kann aber trotzdem hilfreich sein.
  conf.debug_output = true;

  //Das setzt die Übertragungsgeschwindigkeit der SPI Schnittstelle.
  //Falls hohe Werte gesetzt werden, sollte eine gute Verbingung sichergestellt werden.
  conf.spiTransferSpeed = 600000;

  //Initialisiere den LedController mit der erstellten Konfiguration.
  control.init(conf);

  //setzt die Helligkeit auf die niedrigste Helligkeit
  control.setIntensity(0);

  //schaltet alle Ziffern aus
  for(unsigned int i = 0; i < 8;i++){
    control.setRow(0,i,0x00);
    control.setRow(1,i,0x00);
  }
}

void loop() {
  //Verzögerung zwischen den Messungen
  delay(delayMS);

  //Temperatur holen und auf der oberen Reihe anzeigen anzeigen
  auto temp = getTemp();
  Serial.print(F("Temperature: "));
  Serial.println(temp);
  displayFloat(temp,1,1);
  control.setChar(1,7,'t',false);

  //Luftfeuchtigkeit holen und auf der unteren Reihe anzeigen anzeigen
  auto humid = getHumidity();
  Serial.print(F("Humidity: "));
  Serial.println(humid);
  displayFloat(humid,0,1);
  control.setChar(0,7,'H',false);
  
}
