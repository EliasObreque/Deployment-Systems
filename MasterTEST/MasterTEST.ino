#include <Wire.h>

#define START_SENSORS_TEMP 30
#define STOP_SENSORS_TEMP 31
#define GET_TEMP 32
#define READ_SW_FACE 33
#define BURN_FACE 34
#define SET_BURN 35
#define SENSORS_TEMP_STATUS 36


const byte I2C_SLAVE_ADDR = 0x40;
String cmd_name;
String cmd_value;
bool cmd_change = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 	while (Serial.available()){
 	delay(4);
 	if (Serial.available() > 0) {
 		char c = Serial.read();  //gets one byte from serial buffer
	    if (c == ' ' || c == '\n'){
	    	cmd_change = 1;
	    }
	    else {
	    	if (cmd_change == 0) {
	    	cmd_name += c; //makes the string readString
	    }
	  	else {
	  		cmd_value += c;
	  	}
   		}
	}
}
  if (cmd_name.length() > 0) {
    Serial.print(cmd_name); Serial.print("\t"); Serial.println(cmd_value);
    sendCom(cmd_name, cmd_value);
    reset_i2c_cmd();
	}
    delay(500);
}

void reset_i2c_cmd(){
	cmd_name = "";
  cmd_value = "";
  cmd_change = 0;

}

void sendCom(String data, String value){
	// Convert from String Object to String.
	int selected_cmd = 0;
	int selected_value = 0; // {1: x+, 2: x-, 3: y+, 4 y-}

	if (data == "is2_start_sensors"){
		Serial.println("cmd start");
		selected_cmd = START_SENSORS_TEMP;
    	selected_value = 0;
    }
    else if (data == "is2_stop_sensors"){
    	Serial.println("cmd stop");
		selected_cmd = STOP_SENSORS_TEMP;
    	selected_value = 0;	
   	}
	else if (data == "is2_get_state_panel") {
  		Serial.print("cmd read sw");
		selected_cmd = READ_SW_FACE;
		selected_value = int(value[0]) - 48;
		Serial.println(selected_value);
	}
	else if (data == "is2_deploy_panel"){
		selected_cmd = BURN_FACE;
		selected_value = int(value[0]);
	}
	else if (data == "is2_set_deploy"){
		selected_cmd = SET_BURN;
    	selected_value = 0;
	}
	else if (data == "is2_get_temp"){
  		Serial.println("cmd get temp");
		selected_cmd = GET_TEMP;
  		selected_value = 0;
	}
	else if (data == "is2_get_sensors_status"){
		Serial.println("get sensors status");
		selected_cmd = SENSORS_TEMP_STATUS;
  		selected_value = 0;
	}
	
	Wire.beginTransmission(I2C_SLAVE_ADDR); // transmit to device #40
  	Wire.write((int) selected_cmd);        // sends five bytes
  	Wire.write((int) selected_value);              // sends one byte  
  	Wire.endTransmission();    // stop transmitting

   request_to_slave(selected_cmd);
}

void request_to_slave(int selected_cmd){
	if (selected_cmd == READ_SW_FACE){
		Wire.requestFrom(I2C_SLAVE_ADDR, 2);
	    int face = Wire.read();
	    int state = Wire.read();
	    Serial.println("read sw");
	    Serial.print((int) face); Serial.print("\t"); Serial.println(state);
  	}
  	else if (selected_cmd == START_SENSORS_TEMP){
		int state = Wire.read();
		Serial.println("start temp");
	Serial.println(state);
   	} 
   	else if (selected_cmd == GET_TEMP){
	    const int VEC_MAX = 2;
	    float vec[VEC_MAX];
	 
	    Wire.requestFrom(I2C_SLAVE_ADDR, VEC_MAX * sizeof(float));

    	for (int i = 0; i < VEC_MAX; i++){
    		byte buff[VEC_MAX * sizeof(float)];
			buff[0] = Wire.read();
			buff[1] = Wire.read();
			buff[2] = Wire.read();
			buff[3] = Wire.read();

			vec[i] = *((float*)(buff));
    		Serial.println(vec[i]);
    	}
    	
    	//for (int i = 0; i < VEC_MAX; i++) {
  		//	Serial.println(vec_char[i]);
    	//}
	}
    else if (selected_cmd == SENSORS_TEMP_STATUS){
    	Wire.requestFrom(I2C_SLAVE_ADDR, 1);
    	int status = Wire.read();
    	Serial.println("status of sensors");
    	Serial.println(status);
    }
}

float getFloat(byte packet[], int i)
{
    float out;
    memcpy(&out, &packet[i], sizeof(float));

    return out;
}
