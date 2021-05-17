#include <Wire.h>

#define INIT_SENSORS_TEMP 30
#define GET_TEMP 31
#define READ_SW_FACE 32
#define BURN_FACE 33
#define SET_BURN 34

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
		Serial.println(INIT_SENSORS_TEMP);
		selected_cmd = INIT_SENSORS_TEMP;}
	else if (data == "is2_get_state_panel") {
		selected_cmd = READ_SW_FACE;
		selected_value = int(value[0]);
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
	
	Wire.beginTransmission(0x40); // transmit to device #40
  	Wire.write(selected_cmd);        // sends five bytes
  	Wire.write(selected_value);              // sends one byte  
  	Wire.endTransmission();    // stop transmitting
  	delay(500);
}
