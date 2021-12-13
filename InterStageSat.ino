/**************************************************************************/
/*
Elias Obreque S.

els.obrq@gmail.com
InterStage Custom v3
*/
/**************************************************************************/

#include "InterStageSat.h"

void setup() {

  // debug usb
  Serial.begin(9600);
  Serial.print("Started ...");

  setup_temp_sensors();

  if (asSlave){
    Serial.println(" as Slave");
    Wire.begin(I2C_SLAVE_ADDR);               
    Wire.onReceive(readMasterWrite);
    Wire.onRequest(responseToMasterRead);
  };

  setup_burning();
}

void loop() {

  if (asSlave){
    run_loop_for_temp_sensors();

    run_loop_for_burn_resistor();
  }
  else {
    read_sensors();
    print_data_sensors();
    read_sw_state(1);
    activate_resistor(1);
  }
  delay(100);
}

void readMasterWrite(int howMany){
  cmd_num = Wire.read();
  cmd_value = Wire.read();    // receive byte as an integer
  
  //Serial.print(cmd_num); Serial.print("\t"); Serial.println(cmd_value);

  cmd2Term(cmd_num, cmd_value);

  cmd2Burn(cmd_num, cmd_value);
  }

void responseToMasterRead(){

  //if (cmd_num == START_SENSORS_TEMP || cmd_num == STOP_SENSORS_TEMP){
  response_from_Temp(cmd_num);
  //}
  
  
  response_from_Burn(cmd_num, cmd_value);
}

