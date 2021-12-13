
#include "Adafruit_MCP9808.h"
#include "Temp_sensors.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 temp_array[4];

/*Adafruit_MCP9808 t1 = Adafruit_MCP9808();
Adafruit_MCP9808 t2 = Adafruit_MCP9808();
Adafruit_MCP9808 t3 = Adafruit_MCP9808();
Adafruit_MCP9808 t4 = Adafruit_MCP9808();
Adafruit_MCP9808 t5 = Adafruit_MCP9808();
Adafruit_MCP9808 t6 = Adafruit_MCP9808();
Adafruit_MCP9808 t7 = Adafruit_MCP9808();
Adafruit_MCP9808 t8 = Adafruit_MCP9808();*/

// temp
const int num_temp_sensor = 4;
float current_temp[num_temp_sensor];

// flags 
int flag_i2c_error_temp[4];
int flag_i2c_start_temp = 0;
int flag_i2c_stop_temp = 0;
int flag_i2c_sample_temp = 0;
int array_addr_num[4];

union temp_sensor_Union  
{
  float temp_Float;
  unsigned char temp_Bytes[4];
};

void setup_temp_sensors(int array_addr[]){
  for (int i=0; i < sizeof(array_addr); i++){
    if (array_addr[i] < 6){ 
      temp_array[i] = Adafruit_MCP9808();
      array_addr_num[i] = array_addr[i];
    }
  }

	Serial.print("Init sensor ...");
	init_i2c_sensor();
	wake_sensors();
}

void run_loop_for_temp_sensors(){
	if (flag_i2c_start_temp == 1){
		//wake up MCP9808 - power consumption ~200 mikro Ampere
		wake_sensors();
		flag_i2c_start_temp = 0;
	}
	if (flag_i2c_stop_temp == 1){
		shutdown_wake_sensor();
		flag_i2c_stop_temp = 0;
	}
	if (flag_i2c_sample_temp == 1){
		read_sensors();
		flag_i2c_sample_temp = 0;
	}
}

void cmd2Term(int cmd_num,int  cmd_value){
	if (cmd_num == START_SENSORS_TEMP){     // 30
	    flag_i2c_start_temp = 1;
	}
	else if (cmd_num == STOP_SENSORS_TEMP){ // 31
		flag_i2c_stop_temp = 1;
	}
	else if (cmd_num == GET_TEMP){          // 32
	}
	else if (cmd_num == SENSORS_TEMP_STATUS) { // 36
	}
	else if (cmd_num == SAMPLE_TEMP){       //37
	flag_i2c_sample_temp = 1;
	}
}

void response_from_Temp(int cmd_num){
	if (cmd_num == START_SENSORS_TEMP){  // 30
		Wire.write(0);
	}
	else if (cmd_num == STOP_SENSORS_TEMP){
		Wire.write(0);
	}
	else if (cmd_num == GET_TEMP){ // 32
		Wire.write((byte *) current_temp, sizeof(current_temp));
	}
	else if (cmd_num == SENSORS_TEMP_STATUS){
		Wire.write((byte *) flag_i2c_error_temp, sizeof(flag_i2c_error_temp));
	}
	else if (cmd_num == SAMPLE_TEMP){
		Wire.write(0);
	}
}


void print_data_sensors(){
  for (int i=0; i < 2; i++){
    Serial.print(current_temp[i], 4); Serial.print("\t");
    }
  Serial.print("\n"); 
}

void read_sensors(){ 
  for (int i = 0; i < sizeof(array_addr_num); i++){
    if (flag_i2c_error_temp[i] == 0){
      current_temp[i] = temp_array[i].readTempC();
    }  
  }
}

void wake_sensors(){
  for (int i = 0; i < sizeof(array_addr_num); i++){
    if (flag_i2c_error_temp[i] == 0){
      temp_array[i].wake();
    }
  }
}

void shutdown_wake_sensor(){
  for (int i = 0; i < sizeof(array_addr_num); i++){
    if (flag_i2c_error_temp[i] == 0){
      temp_array[i].shutdown_wake(1);
    }
  }
}

void get_resolution(){
  Serial.println (temp_array[0].getResolution());
  }

void init_i2c_sensor(){
  for (int i = 0; i < sizeof(array_addr_num); i++){
    if (!temp_array[i].begin(SENSOR_ADDR_ARRAY[array_addr_num[i]])) {
      flag_i2c_error_temp[i] = 1;
    }
  }
    set_temp_resolution();
  }
  
void set_temp_resolution(){   
  for (int i = 0; i < sizeof(array_addr_num); i++){
    if (flag_i2c_error_temp[i] == 0){
      temp_array[i].setResolution(3);  
    }
  }  
}

void check_i2c_sensor(){
  for (int i = 0; i < sizeof(array_addr_num); i++){  
    if (!temp_array[i].begin(SENSOR_ADDR_ARRAY[array_addr_num[i]])) {
      Serial.println(array_addr_num[i]);
    }
  }
  }
