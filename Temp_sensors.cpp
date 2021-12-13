
#include "Adafruit_MCP9808.h"
#include "Temp_sensors.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 t1 = Adafruit_MCP9808();
Adafruit_MCP9808 t2 = Adafruit_MCP9808();
Adafruit_MCP9808 t3 = Adafruit_MCP9808();
Adafruit_MCP9808 t4 = Adafruit_MCP9808();
Adafruit_MCP9808 t5 = Adafruit_MCP9808();
Adafruit_MCP9808 t6 = Adafruit_MCP9808();
Adafruit_MCP9808 t7 = Adafruit_MCP9808();
Adafruit_MCP9808 t8 = Adafruit_MCP9808();

// temp
const int num_temp_sensor = 8;
float current_temp[num_temp_sensor];

// flags 
int flag_i2c_error_temp[num_temp_sensor];
int flag_i2c_start_temp = 0;
int flag_i2c_stop_temp = 0;
int flag_i2c_sample_temp = 0;

union temp_sensor_Union  
{
  float temp_Float;
  unsigned char temp_Bytes[4];
};

void setup_temp_sensors(){
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
  if (flag_i2c_error_temp[0] == 0){
    current_temp[0] = t1.readTempC();
  }
  if (flag_i2c_error_temp[1] == 0){
    current_temp[1] = t2.readTempC();
  }
  if (flag_i2c_error_temp[2] == 0){
    current_temp[2] = t3.readTempC();
  }
  if (flag_i2c_error_temp[3] == 0){
    current_temp[3] = t4.readTempC();
  }
  if (flag_i2c_error_temp[4] == 0){
    current_temp[4] = t5.readTempC();
  }
  if (flag_i2c_error_temp[5] == 0){
    current_temp[5] = t6.readTempC();
  }
  if (flag_i2c_error_temp[6] == 0){
    current_temp[6] = t7.readTempC();
  }
  if (flag_i2c_error_temp[7] == 0){
    current_temp[7] = t8.readTempC();
  }
}

void wake_sensors(){
  if (flag_i2c_error_temp[0] == 0){
    t1.wake();
  }
  if (flag_i2c_error_temp[1] == 0){
    t2.wake();
  }
  if (flag_i2c_error_temp[2] == 0){
    t3.wake();
  }
  if (flag_i2c_error_temp[3] == 0){
    t4.wake();
  }
  if (flag_i2c_error_temp[4] == 0){
    t5.wake();
  }
  if (flag_i2c_error_temp[5] == 0){
    t6.wake();
  }
  if (flag_i2c_error_temp[6] == 0){
    t7.wake();
  }
  if (flag_i2c_error_temp[7] == 0){
    t8.wake();
  }
}

void shutdown_wake_sensor(){
  if (flag_i2c_error_temp[0] == 0){
    t1.shutdown_wake(1);
  }
  if (flag_i2c_error_temp[1] == 0){
    t2.shutdown_wake(1);
  }
  if (flag_i2c_error_temp[2] == 0){
    t3.shutdown_wake(1);
  }
  if (flag_i2c_error_temp[3] == 0){
    t4.shutdown_wake(1);
  }
  if (flag_i2c_error_temp[4] == 0){
    t5.shutdown_wake(1);
  }
  if (flag_i2c_error_temp[5] == 0){
    t6.shutdown_wake(1);
  }
  if (flag_i2c_error_temp[6] == 0){
    t7.shutdown_wake(1);
  }
  if (flag_i2c_error_temp[7] == 0){
    t8.shutdown_wake(1);
  }
}

void get_resolution(){
  Serial.println (t1.getResolution());
  }

void init_i2c_sensor(){
    if (!t1.begin(0x18)) {
      flag_i2c_error_temp[0] = 1;
    }
    if (!t2.begin(0x19)) {
      flag_i2c_error_temp[1] = 1;
    }
    if (!t3.begin(0x1A)) {
      flag_i2c_error_temp[2] = 1;
    }
    if (!t4.begin(0x1B)) {
      flag_i2c_error_temp[3] = 1;
    }
    if (!t5.begin(0x1C)) {
      flag_i2c_error_temp[4] = 1;
    }
    if (!t6.begin(0x1D)) {
      flag_i2c_error_temp[5] = 1;
    }
  if (!t7.begin(0x1F)) {
      flag_i2c_error_temp[6] = 1;
    }
    if (!t8.begin(0x1F)) {
      flag_i2c_error_temp[7] = 1;
    }
    set_temp_resolution();
  }
  
void set_temp_resolution(){   
  if (flag_i2c_error_temp[0] == 0){
    t1.setResolution(3);  
  } 
  if (flag_i2c_error_temp[1] == 0){
    t2.setResolution(3);  
  } 
  if (flag_i2c_error_temp[2] == 0){
    t3.setResolution(3);  
  } 
  if (flag_i2c_error_temp[3] == 0){
    t4.setResolution(3);  
  } 
  if (flag_i2c_error_temp[4] == 0){
    t5.setResolution(3);  
  } 
  if (flag_i2c_error_temp[5] == 0){
    t6.setResolution(3);  
  } 
  if (flag_i2c_error_temp[6] == 0){
    t7.setResolution(3);  
  } 
  if (flag_i2c_error_temp[7] == 0){
    t8.setResolution(3);  
  }  
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
  if (!t7.begin(0x1F)) {
      Serial.print("7 "); Serial.println("1");
    }
    if (!t8.begin(0x1F)) {
      Serial.print("8 "); Serial.println("1");
    }
  }
