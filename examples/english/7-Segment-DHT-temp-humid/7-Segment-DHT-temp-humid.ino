// DHT Temperature & Humidity Sensor displayed on a 7-Seegment Display

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
// - LedController: https://github.com/noah1510/LedController

//This code is based on the example for the DHT Sensor Library.
//All of the sensor related code is moved into functions to better show the integration of the LedController library.

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 17     // Digital pin connected to the DHT sensor 

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

//Like alway the library has to be included
#include "LedController.hpp"

//The variable that is used to store the Controller object.
//The controller has two rows with one Column each.
//Since 7-Segment-Displays are used, every Segment has 8 Disigts that can be addressed.
LedController<1,2> control = LedController<1,2>();

//The chip select Pins for each row of the LedController
#define CS_BOTTOM 16
#define CS_TOP 22

//The number of digits used to display each float
const unsigned int NUMBER_OF_DIGITS = 4;

//The number of values used to smooth the measured values
#define SMOOTHING_BUFFER_SIZE 10

//This function returns the raw output from the sensor.
//The unit is degrees Celsius.
//Because of this the retrun values might not represent the actual temperature.
//If there was an error reading the value, a value lower than absolute zero in Celsius is returned (less than -273.15) is returned.
float getRawTemp(){
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    return -1000;
  }

  return event.temperature;
}

//This function returns the raw output from the sensor.
//The relative humidity is returned so the value should be between 0 and 100.
//Because of this the retrun values might not represent the actual humidity.
//If there was an error reading the value a negative number is returned.
float getRawHumid(){
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    return -1;
  }
  return event.relative_humidity;
}

//This function returns a smoothed valued for the temperature.
//The unit is degrees Celsius.
//If you expact fast changes use the raw data and handle the invalid values manually.
//Its return value is always valid but only good after SMOOTHING_BUFFER_SIZE calls.
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

//This function returns a smoothed valued for the humidity.
//The relative humidity is returned so the value should be between 0 and 100.
//If you expact fast changes use the raw data and handle the invalid values manually.
//Its return value is always valid but only good after SMOOTHING_BUFFER_SIZE calls.
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

//This function can display a float on a 7-Segment display.
//It assumes the dismension of the LedController is <1,row> and the row can be selected with the second parameter.
//The third parameter is used to specify how many digits you want to after the decimal Place.
//The fourth paramether is an offset moving all digits to the left by the specified amount
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

//this simply prints the sensor information to the serial interface
void printSensorInfo(){
  sensor_t sensor;

  // Print temperature sensor details.
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

  // Print humidity sensor details.
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
  //Initialize the Serial transfer
  Serial.begin(19200);
  // Initialize temperature/humidity device.
  dht.begin();
  
  // Set delay between sensor readings based on sensor details.
  sensor_t sensor;
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  //configure the LedController for the output
  controller_configuration<1,2> conf;

  //disable virtual_muti_row
  conf.SPI_CS = 0;
  conf.virtual_multi_row = false;

  //These are the chip select pins for each row.
  //The bottom row (row 0) is connected to pin 16 and the top row (row 1) is connected to pin 22
  conf.row_SPI_CS[0] = CS_BOTTOM;
  conf.row_SPI_CS[1] = CS_TOP;

  //this enables hardware spi check what pins to use for your board
  conf.useHardwareSpi = true;

  //this enables debug output (nothing should be printed but it helps to fix possible problems with the config)
  conf.debug_output = true;

  //this specifies the transfer speed of the spi interface. If you want to use high values make sure your cables have a good connection
  conf.spiTransferSpeed = 600000;

  //initilizing the LedController with the configuration which we just set
  control.init(conf);

  //set the lowest possible brightness
  control.setIntensity(0);

  //turn every digit off
  for(unsigned int i = 0; i < 8;i++){
    control.setRow(0,i,0x00);
    control.setRow(1,i,0x00);
  }
}

void loop() {
  // Delay between measurements.
  delay(delayMS);

  //get and display the temperature in the top row
  auto temp = getTemp();
  Serial.print(F("Temperature: "));
  Serial.println(temp);
  displayFloat(temp,1,1);
  control.setChar(1,7,'t',false);

  //get and display the humidity in the bottom row
  auto humid = getHumidity();
  Serial.print(F("Humidity: "));
  Serial.println(humid);
  displayFloat(humid,0,1);
  control.setChar(0,7,'H',false);
  
}
