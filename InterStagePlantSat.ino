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

#define START_SENSORS_TEMP 30
#define STOP_SENSORS_TEMP 31
#define GET_TEMP 32
#define READ_SW_FACE 33
#define BURN_FACE 34
#define SET_BURN 35
#define SENSORS_TEMP_STATUS 36

#define pin_sw_x_plus 8 
#define pin_sw_x_minus 4 
#define pin_sw_y_plus 7 
#define pin_sw_y_minus 2 

#define pin_burn_x_plus 3
#define pin_burn_x_minus 6
#define pin_burn_y_plus 5
#define pin_burn_y_minus 9

const byte I2C_SLAVE_ADDR = 0x40;

int cmd_num = 10;
int cmd_value = 5;

const int num_temp_sensor = 2;
float current_temp[num_temp_sensor];
int state_sw = 0;
int flag_i2c_error_temp = 0;

unsigned long current_time;
int period_time = 3000; // milisecond
int attempt_burn = 5;
byte outputValue = 50;
byte increment_per_attempt = 20; 

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
void read_sw_state();
union temp_sensor_Union  
{
      float temp_Float;
      unsigned char temp_Bytes[4];
};


void setup() {

  // debug usb
  Serial.begin(9600);
  
  init_i2c_sensor();

  wake_sensors();
  Wire.begin(I2C_SLAVE_ADDR);               
  Wire.onReceive(readMasterWrite);
  Wire.onRequest(responseToMasterRead);
  
  pinMode(pin_sw_x_plus, INPUT);
  pinMode(pin_sw_y_minus, INPUT);
  pinMode(pin_sw_y_plus, INPUT);
  pinMode(pin_sw_y_minus, INPUT);

  pinMode(pin_burn_x_plus, OUTPUT);
  pinMode(pin_burn_x_minus, OUTPUT);
  pinMode(pin_burn_y_plus, OUTPUT);
  pinMode(pin_burn_y_minus, OUTPUT);
  
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F
    
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
}

void loop() {
  //wake up MCP9808 - power consumption ~200 mikro Ampere
  //wake_sensors();
  //read_sensors();
  //print_data_sensors();
  //delay(2000);
  //shutdown_wake_sensor();
  //int face = 1;
  //activate_resistor(face);
  //Serial.println("waiting...");
  delay(100);
}

void readMasterWrite(int howMany){
  cmd_num = Wire.read();
  cmd_value = Wire.read();    // receive byte as an integer
  
  if (cmd_num == START_SENSORS_TEMP){
    wake_sensors();
  }
  else if (cmd_num == STOP_SENSORS_TEMP){
    shutdown_wake_sensor();
  }
  else if (cmd_num == BURN_FACE){
    activate_resistor(cmd_value);
  }
  else if (cmd_num == GET_TEMP){
    read_sensors();
  }
  else if (cmd_num == READ_SW_FACE){
    read_sw_state(cmd_value);
  }
  }

void responseToMasterRead(){
  if (cmd_num == START_SENSORS_TEMP){
    if (flag_i2c_error_temp == 0){
      Wire.write(flag_i2c_error_temp);
    }
    else {
      Wire.write(flag_i2c_error_temp);
    }
  }
  else if (cmd_num == GET_TEMP){
    current_temp[0] = 123.1;
    current_temp[1] = 11.1;
    Wire.write((byte *) current_temp, sizeof(current_temp));
  }
  else if (cmd_num == READ_SW_FACE){
    Wire.write((int) cmd_value);
    Wire.write((int) state_sw);
  }
  else if (cmd_num == SENSORS_TEMP_STATUS){
    Wire.write(flag_i2c_error_temp);
  }
}

void read_sw_state(int face){
  if (face == 1){
    state_sw = digitalRead(pin_sw_x_plus);
  }
  else if (face == 2){
    state_sw = digitalRead(pin_sw_y_plus);
  }
  else if (face == 3){
    state_sw = digitalRead(pin_sw_x_minus);
  }
  else if (face == 4){
    state_sw = digitalRead(pin_sw_y_minus);
  }
}

void activate_resistor(int face){
  int selected_pin;
  bool active = false;
  if (face == 1){
    if (digitalRead(pin_sw_x_plus) == 1){
      selected_pin = pin_burn_x_plus;
    }
  }
  else if (face == 3){
    if (digitalRead(pin_sw_x_minus) == 1){
      selected_pin = pin_burn_x_minus;
    }
  }
  else if (face == 2){
    if (digitalRead(pin_sw_y_plus) == 1){
      selected_pin = pin_burn_y_plus;
    }
  }
  else if (face == 4){
    if (digitalRead(pin_sw_y_minus) == 1){
      selected_pin = pin_burn_y_minus;
    }
  }  

  if (active) {
    for (int i=0; i < attempt_burn; i++){
      current_time = millis();
      Serial.print("Attempt"); Serial.print("\t");
      Serial.println(i);
      while (millis() < current_time + period_time){
        digitalWrite(selected_pin, HIGH);
      }
      digitalWrite(selected_pin, LOW);
      delay(int (period_time / 2));
    }
  }
}

void print_data_sensors(){
  for (int i=0; i < 2; i++){
    Serial.print(current_temp[i], 4); Serial.print("\t");
    }
  //Serial.print(current_temp[7], 4); Serial.println("\t"); 
}

void read_sensors(){    
  current_temp[0] = (int)(t1.readTempC()*1000);
  current_temp[1] = (int)(t2.readTempC()*1000);
  /*current_temp[2] = (int)(t3.readTempC()*1000);
  current_temp[3] = (int)(t4.readTempC()*1000);
  current_temp[4] = (int)(t5.readTempC()*1000);
  current_temp[5] = (int)(t6.readTempC()*1000);
  current_temp[6] = (int)(t7.readTempC()*1000);
  current_temp[7] = (int)(t8.readTempC()*1000);  
  */}

void wake_sensors(){
  if (flag_i2c_error_temp == 0){
    t1.wake();
    t2.wake();
    /*t3.wake();
    t4.wake();
    t5.wake();
    t6.wake();
    t7.wake();
    t8.wake();
  */}
}

void shutdown_wake_sensor(){
  t1.shutdown_wake(1);
  t2.shutdown_wake(1);
  /*t3.shutdown_wake(1);
  t4.shutdown_wake(1);
  t5.shutdown_wake(1);
  t6.shutdown_wake(1);
  t7.shutdown_wake(1);
  t8.shutdown_wake(1);
  */}

void get_resolution(){
  Serial.println (t1.getResolution());
  }

void init_i2c_sensor(){
    if (!t1.begin(0x18)) {
      flag_i2c_error_temp = 1;
    }
    if (!t2.begin(0x19)) {
      flag_i2c_error_temp = 2;
    }
    /*
    if (!t3.begin(0x1A)) {
      flag_i2c_error_temp = 3;
    }
    if (!t4.begin(0x1B)) {
      flag_i2c_error_temp = 4;
    }
    if (!t5.begin(0x1C)) {
      flag_i2c_error_temp = 5;
    }
    if (!t6.begin(0x1D)) {
      flag_i2c_error_temp = 6;
    }
  if (!t7.begin(0x1E)) {
      flag_i2c_error_temp = 7;
    }
    if (!t8.begin(0x1F)) {
      flag_i2c_error_temp = 8;
    }
    */
      t1.setResolution(3);
  t2.setResolution(3);
  /*t3.setResolution(3);
  t4.setResolution(3);
  t5.setResolution(3);
  t6.setResolution(3);
  t7.setResolution(3);
  t8.setResolution(3);
  */
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


