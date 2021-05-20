#include <Wire.h>

#define START_SENSORS_TEMP 30
#define STOP_SENSORS_TEMP 31
#define GET_TEMP 32
#define READ_SW_FACE 33
#define BURN_FACE 34
#define SET_BURN 35

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
	    if (c == ' '){
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
		Serial.print(cmd_name);
		Serial.print(cmd_value);
		sendCom(cmd_name, cmd_value);
		reset_i2c_cmd();
	}
}

void reset_i2c_cmd(){
	cmd_name = "";
   	cmd_value = "";
   	cmd_change = 0;

}

void sendCom(String data, String value){
	// Convert from String Object to String.
	int selected_cmd;
	int selected_value = 0; // {1: x+, 2: x-, 3: y+, 4 y-}

	if (data == "is2_init_sensor"){
		Serial.println(START_SENSORS_TEMP);
		selected_cmd = START_SENSORS_TEMP;}
	else if (data == "is2_get_state_panel") {
		selected_cmd = READ_SW_FACE;
		selected_value = int(value[0]) - 48;
   Serial.print(selected_cmd); Serial.print("\t"); Serial.println(selected_value);
	}
	else if (data == "is2_deploy_panel"){
		selected_cmd = BURN_FACE;
		selected_value = int(value[0]);
	}
	else if (data == "is2_set_deploy"){
		selected_cmd = SET_BURN;
	}
	else if (data == "is2_get_temp"){
	selected_cmd = GET_TEMP;
	}
	
	  Wire.beginTransmission(I2C_SLAVE_ADDR); // transmit to device #40
  	Wire.write(selected_cmd);        // sends five bytes
  	Wire.write(selected_value);              // sends one byte  
  	Wire.endTransmission();    // stop transmitting

  if (selected_cmd == READ_SW_FACE){
    Wire.requestFrom(I2C_SLAVE_ADDR, 2);
    int face = Wire.read();
    int state = Wire.read();
    Serial.print(face); Serial.print("\t"); Serial.println(state);
    } 
   else if (selected_cmd == GET_TEMP){
    const int VEC_MAX = 8;
    float vec[VEC_MAX];
    uint8_t* vp = (uint8_t*) vec;
    Wire.requestFrom(I2C_SLAVE_ADDR, 8);
    while (Wire.available()) {
      *vp++ = Wire.read();
    }
    for (int i = 0; i < VEC_MAX; i++) {
      Serial.println(vec[i]);
    }
   }
  delay(500);
}
