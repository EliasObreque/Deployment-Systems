/**************************************************************************/
/*
Elias Obreque S.

els.obrq@gmail.com
InterStage B Custom v3
*/
/**************************************************************************/


#include "InterStageSat_VB.h"

PhotoDiodeFSS sensor_fss; 

void setup() {

  // debug usb
  Serial.begin(9600);
  Serial.print("Started ...");
  
  if (asSlave){
    Serial.println(" as Slave");
    Wire.begin(I2C_SLAVE_ADDR_B);               
    Wire.onReceive(readMasterWrite);
    Wire.onRequest(responseToMasterRead);
  }else{Serial.println(" as Master"); Wire.begin();}

  setup_burning();
  sensor_fss.Mode();
  sensor_fss.setDefault();
}

void loop() {

  if (asSlave){
    run_loop_for_burn_resistor();
    sensor_fss.run_loop_for_fss();
  }
  else {
    read_sw_state(1);
    activate_resistor(1);

    sensor_fss.read();
    sensor_fss.calc_sun_position();
    sensor_fss.get_sun_vector();
    delay(500);
  }
  delay(100);
}

void readMasterWrite(int howMany){
  cmd_num   = Wire.read();
  cmd_value = Wire.read();    // receive byte as an integer
  
  //Serial.print(cmd_num); Serial.print("\t"); Serial.println(cmd_value);
  cmd2Burn(cmd_num, cmd_value);
  sensor_fss.cmd2FSS(cmd_num);
  }

void responseToMasterRead(){  
  response_from_Burn(cmd_num, cmd_value);
  sensor_fss.response_from_FSS(cmd_num); 
}

