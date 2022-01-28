
#include "Adafruit_MCP9808.h"
#include "Temp_sensors.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 temp_array[num_temp_sensor];

float current_temp[num_temp_sensor];

// flags 
int flag_i2c_error_temp[num_temp_sensor];
int flag_i2c_start_temp = 0;
int flag_i2c_stop_temp = 0;
int flag_i2c_sample_temp = 0;
int array_addr_num[num_temp_sensor];

union temp_sensor_Union  
{
  float temp_Float;
  unsigned char temp_Bytes[4];
};

void setup_temp_sensors(int array_addr[]){
  for (int i=0; i < num_temp_sensor; i++){
    if (array_addr[i] < 6){ 
      temp_array[i] = Adafruit_MCP9808();
      array_addr_num[i] = array_addr[i];
    }
  }

	Serial.println("Init sensor ...");
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
	Serial.println(flag_i2c_sample_temp);
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
		print_data_sensors();
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
  for (int i=0; i < num_temp_sensor; i++){
    Serial.print(current_temp[i], 4); Serial.print("\t");
    }
  Serial.print("\n"); 
}

void read_sensors(){ 
  Serial.print("Reading ...");
  for (int i = 0; i < num_temp_sensor; i++){
  	Serial.print(i);
    Serial.print(flag_i2c_error_temp[i]);
    if (flag_i2c_error_temp[i] == 0){
    	Wire.begin();
      current_temp[i] = temp_array[i].readTempC();
    	Wire.setClock(100000);
    }  
    else{current_temp[i] = 777.0;}
  }
  Serial.print("\n"); 
}

void wake_sensors(){
	Wire.begin();
  for (int i = 0; i < num_temp_sensor; i++){
    if (flag_i2c_error_temp[i] == 0){
      temp_array[i].wake();
    }
  }
  Wire.setClock(100000);
}

void shutdown_wake_sensor(){
	Wire.begin();
  for (int i = 0; i < num_temp_sensor; i++){
    if (flag_i2c_error_temp[i] == 0){
      temp_array[i].shutdown_wake(1);
    }
  }
  Wire.setClock(100000);
}

void get_resolution(){
  Serial.println (temp_array[0].getResolution());
  }

void init_i2c_sensor(){
  Wire.begin();
  for (int i = 0; i < num_temp_sensor; i++){
  	Serial.print(i); Serial.print(" - ");
    if (!temp_array[i].begin(SENSOR_ADDR_ARRAY[array_addr_num[i]])) {
      flag_i2c_error_temp[i] = 1;
      Serial.println(1);
    }else{Serial.println(0); flag_i2c_error_temp[i] = 0;}
  }
    set_temp_resolution();
  Wire.setClock(100000);
  }
  
void set_temp_resolution(){   
  for (int i = 0; i < num_temp_sensor; i++){
    if (flag_i2c_error_temp[i] == 0){
    	Wire.begin();
      temp_array[i].setResolution(3);  
      Wire.setClock(100000);
    }
  }  
}

void check_i2c_sensor(){
  for (int i = 0; i < num_temp_sensor; i++){  
    if (!temp_array[i].begin(SENSOR_ADDR_ARRAY[array_addr_num[i]])) {
      Serial.println(array_addr_num[i]);
    }
  }
  }
