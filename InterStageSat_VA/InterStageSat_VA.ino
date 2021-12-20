/**************************************************************************/
/*
Elias Obreque S.

els.obrq@gmail.com
InterStage A Custom v3
*/
/**************************************************************************/

#include "InterStageSat_VA.h"

PhotoDiodeFSS sensor_fss; 

void setup() {

  // debug usb
  Serial.begin(9600);
  Serial.print("Started ...");
  
  array_temp_selected[0] = SENSOR_A1_1_NUM;
  array_temp_selected[1] = SENSOR_A1_2_NUM;
  array_temp_selected[2] = SENSOR_B1_1_NUM;
  array_temp_selected[3] = SENSOR_B1_2_NUM;
  array_temp_selected[4] = SENSOR_B2_1_NUM;
  array_temp_selected[5] = SENSOR_B2_2_NUM;


  if (asSlave){
    Serial.println(" as Slave");
    Wire.begin(I2C_SLAVE_ADDR_A);               
    Wire.onReceive(readMasterWrite);
    Wire.onRequest(responseToMasterRead);
  }else{
    Serial.println(" as Master");
    Wire.begin();
  }

  setup_temp_sensors(array_temp_selected);
  setup_burning();
  sensor_fss.Mode();
  sensor_fss.setDefault();
}

void loop() {

  if (asSlave){
    run_loop_for_temp_sensors();

    run_loop_for_burn_resistor();
  }
  else {
    //read_sensors();
    //print_data_sensors();
    read_sw_state(1);
    activate_resistor(1);

    sensor_fss.read();
    //sensor_fss.print();
    sensor_fss.calc_sun_position();
    sensor_fss.get_sun_vector();
    delay(500);

  }
  delay(100);
}

void readMasterWrite(int howMany){
  cmd_num = Wire.read();
  cmd_value = Wire.read();    // receive byte as an integer
  
  //Serial.print(cmd_num); Serial.print("\t"); Serial.println(cmd_value);

  cmd2Term(cmd_num, cmd_value);

  cmd2Burn(cmd_num, cmd_value);

  cmd2FSS(cmd_num, cmd_value);
  }

void responseToMasterRead(){

  //if (cmd_num == START_SENSORS_TEMP || cmd_num == STOP_SENSORS_TEMP){
  response_from_Temp(cmd_num);
  //}
  
  
  response_from_Burn(cmd_num, cmd_value);
}

