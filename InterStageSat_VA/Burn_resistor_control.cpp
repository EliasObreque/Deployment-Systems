

#include "Burn_resistor_control.h"
#include <Arduino.h>

int flag_burn_face = 0;

// burn
int face_to_burn;
int current_time;
int period_time = 2000; // milisecond
int attempt_burn = 5;
int state_sw = 0;

void setup_burning(){
	pinMode(pin_sw_x_plus, INPUT);
	pinMode(pin_sw_y_minus, INPUT);
	pinMode(pin_sw_y_plus, INPUT);
	pinMode(pin_sw_y_minus, INPUT);

	pinMode(pin_burn_x_plus, OUTPUT);
	pinMode(pin_burn_x_minus, OUTPUT);
	pinMode(pin_burn_y_plus, OUTPUT);
	pinMode(pin_burn_y_minus, OUTPUT);
}

void run_loop_for_burn_resistor(){
	if (flag_burn_face == 1){
    	activate_resistor(face_to_burn);
    	flag_burn_face = 0;
  }
}

void cmd2Burn(int cmd_num, int cmd_value){
	if (cmd_num == READ_SW_FACE){      // 33
    	read_sw_state(cmd_value);
  	}
  	else if (cmd_num == BURN_FACE){         // 34
    	flag_burn_face = 1;
    	face_to_burn = cmd_value;
  	}
  	else if (cmd_num == SET_BURN){          // 35
    	set_burn_config(cmd_num);
  }
}

void response_from_Burn(int cmd_num, int cmd_value){
	if (cmd_num == READ_SW_FACE){
    	Wire.write((int) cmd_value);
    	Wire.write((int) state_sw);
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
  int active = 0;
  if (face == 1){
    if (digitalRead(pin_sw_x_plus) == 1){
      selected_pin = pin_burn_x_plus;
      active = 1;
    }
  }
  else if (face == 3){
    if (digitalRead(pin_sw_x_minus) == 1){
      selected_pin = pin_burn_x_minus;
      active = 1;
    }
  }
  else if (face == 2){
    if (digitalRead(pin_sw_y_plus) == 1){
      selected_pin = pin_burn_y_plus;
      active = 1;
    }
  }
  else if (face == 4){
    if (digitalRead(pin_sw_y_minus) == 1){
      selected_pin = pin_burn_y_minus;
      active = 1;
    }
  }  
  int dt = 100;
  if (active == 1) {
    for (int i=0; i < attempt_burn; i++){
      current_time = 0;
      // Serial.print("Attempt"); Serial.print("\t");
      // Serial.println(i);
      while (current_time < period_time){
        // Serial.print(current_time);
        read_sw_state(face);
        if (state_sw == 1){
          // Serial.print("h");
          digitalWrite(selected_pin, HIGH);
        }
        else{
          // Serial.print("l");
          digitalWrite(selected_pin, LOW);
          break; 
        }
        delay(dt);
        current_time = current_time + dt;
      }
      read_sw_state(face);
      digitalWrite(selected_pin, LOW);
      delay(int (period_time / 2));
      // Serial.println("d");
      if (state_sw == 0){break;}
    }
  }
}

void set_burn_config(int cmd_value){
  attempt_burn = cmd_value;
  // Serial.println(attempt_burn);
}
