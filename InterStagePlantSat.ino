/**************************************************************************/
/*!
This is a demo for the Adafruit MCP9808 breakout
----> http://www.adafruit.com/products/1782
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!
*/
/**************************************************************************/

#include <Wire.h>
#include "Adafruit_MCP9808.h"
float current_temp[8];

#define START_SENSORS_TEMP 30
#define STOP_SENSORS_TEMP 31
#define GET_TEMP 32
#define READ_SW_FACE 33
#define BURN_FACE 34
#define SET_BURN 35


// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 t1 = Adafruit_MCP9808();
Adafruit_MCP9808 t2 = Adafruit_MCP9808();
Adafruit_MCP9808 t3 = Adafruit_MCP9808();
Adafruit_MCP9808 t4 = Adafruit_MCP9808();
Adafruit_MCP9808 t5 = Adafruit_MCP9808();
Adafruit_MCP9808 t6 = Adafruit_MCP9808();
Adafruit_MCP9808 t7 = Adafruit_MCP9808();
Adafruit_MCP9808 t8 = Adafruit_MCP9808();

void print_data_sensors();
void read_sensors();
void wake_sensors();
void check_i2c_sensor();
void shutdown_wake_sensor();
void get_resolution();


void setup() {
  Wire.begin(0x40);               
  Wire.onReceive(readMasterWrite);
  Wire.onRequest(responseToMasterRead);
  
  // Serial.begin(9600);
  while (!Serial); //waits for serial terminal to be open, necessary in newer arduino boards.
  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example, also can be left in blank for default address use
  // Also there is a table with all addres possible for this sensor, you can connect multiple sensors
  // to the same i2c bus, just configure each sensor with a different address and define multiple objects for that
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F

  //init_i2c_sensor();
  check_i2c_sensor();
 // sets the resolution mode of reading, the modes are defined in the table bellow:
  
  t1.setResolution(3);
  t2.setResolution(3);
  t3.setResolution(3);
  t4.setResolution(3);
  t5.setResolution(3);
  t6.setResolution(3);
  t7.setResolution(3);
  t8.setResolution(3);
  
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
}

void loop() {
  // wake up MCP9808 - power consumption ~200 mikro Ampere
  //wake_sensors();
  //read_sensors();
  //print_data_sensors();
  //delay(2000);
  //shutdown_wake_sensor();
  delay(200);
}

void readMasterWrite(int howMany){
  int cmd_num = Wire.read();
  int cmd_value = Wire.read();    // receive byte as an integer

  if (cmd_num == INIT_SENSORS_TEMP){
    wake_sensors();
  }
  else if (cmd_num == STOP_SENSORS_TEMP){
    shutdown_wake_sensor();
  }
  else if (cmd_num == BURN_FACE){

  }
  else if (cmd_num == SET_BURN){
    
  }
  }

void responseToMasterRead(){
  
  }

void print_data_sensors(){
  for (int i=0; i < 7; i++){
    Serial.print(current_temp[i], 4); Serial.print("\t");
    }
  Serial.print(current_temp[7], 4); Serial.println("\t"); 
}

void read_sensors(){
  current_temp[0] = t1.readTempC();
  current_temp[1] = t2.readTempC();
  current_temp[2] = t3.readTempC();
  current_temp[3] = t4.readTempC();
  current_temp[4] = t5.readTempC();
  current_temp[5] = t6.readTempC();
  current_temp[6] = t7.readTempC();
  current_temp[7] = t8.readTempC();  
  }

void wake_sensors(){
  t1.wake();
  t2.wake();
  t3.wake();
  t4.wake();
  t5.wake();
  t6.wake();
  t7.wake();
  t8.wake();

  check_i2c_sensor();
  }

void shutdown_wake_sensor(){
  t1.shutdown_wake(1);
  t2.shutdown_wake(1);
  t3.shutdown_wake(1);
  t4.shutdown_wake(1);
  t5.shutdown_wake(1);
  t6.shutdown_wake(1);
  t7.shutdown_wake(1);
  t8.shutdown_wake(1);
  }

void get_resolution(){
  Serial.println (t1.getResolution());
  }

void init_i2c_sensor(){
  }

void check_i2c_sensor(){
    if (!t1.begin(0x18)) {
      Serial.print("1 "); Serial.println("1");
    }
    if (!t2.begin(0x19)) {
      Serial.print("2 "); Serial.println("1");
    }
    if (!t3.begin(0x1A)) {
      Serial.print("3 "); Serial.println("1");
    }
    if (!t4.begin(0x1B)) {
      Serial.print("4 "); Serial.println("1");
    }
    if (!t5.begin(0x1C)) {
      Serial.print("5 "); Serial.println("1");
    }
    if (!t6.begin(0x1D)) {
      Serial.print("6 "); Serial.println("1");
    }
  if (!t7.begin(0x1E)) {
      Serial.print("7 "); Serial.println("1");
    }
    if (!t8.begin(0x1F)) {
      Serial.print("8 "); Serial.println("1");
    }
  }
