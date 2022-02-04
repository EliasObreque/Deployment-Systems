#include <Wire.h>

// temp
const int num_temp_sensor = 6;

  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F

int array_temp_selected[num_temp_sensor] = {7, 7, 7, 7, 7, 7};

const byte SENSOR_A1_1_ADDR = 0x18;
const byte SENSOR_A1_2_ADDR = 0x19;
const byte SENSOR_B1_1_ADDR = 0x1A;
const byte SENSOR_B1_2_ADDR = 0x1B;
const byte SENSOR_B2_1_ADDR = 0x1C;
const byte SENSOR_B2_2_ADDR = 0x1D;


const byte SENSOR_ADDR_ARRAY[num_temp_sensor] = {SENSOR_A1_1_ADDR, SENSOR_A1_2_ADDR, SENSOR_B1_1_ADDR, SENSOR_B1_2_ADDR, SENSOR_B2_1_ADDR, SENSOR_B2_2_ADDR};
    
const int SENSOR_A1_1_NUM = 0;
const int SENSOR_A1_2_NUM = 1;
const int SENSOR_B1_1_NUM = 2;
const int SENSOR_B1_2_NUM = 3;
const int SENSOR_B2_1_NUM = 4;
const int SENSOR_B2_2_NUM = 5;

//array_temp_selected[0] = SENSOR_A1_1_NUM;
//array_temp_selected[1] = SENSOR_A1_2_NUM;
//array_temp_selected[2] = SENSOR_B1_1_NUM;
//array_temp_selected[3] = SENSOR_B1_2_NUM;
//array_temp_selected[4] = SENSOR_B2_1_NUM;
//array_temp_selected[5] = SENSOR_B2_2_NUM;


  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms

#define START_SENSORS_TEMP 30
#define STOP_SENSORS_TEMP 31
#define GET_TEMP 32
#define SENSORS_TEMP_STATUS 36
#define SAMPLE_TEMP 37

void print_data_sensors();
void read_sensors();
void wake_sensors();
void check_i2c_sensor();
void shutdown_wake_sensor();
void get_resolution();
void init_i2c_sensor();
union temp_sensor_Union;
void setup_temp_sensors(int array_addr[]);
void set_temp_resolution();
void run_loop_for_temp_sensors();
void cmd2Term(int cmd_num, int cmd_value);
void response_from_Temp(int cmd_num);
