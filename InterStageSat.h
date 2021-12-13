#include <Wire.h>
#include "Temp_sensors.h"
#include "Burn_resistor_control.h"

//A1
//const byte I2C_SLAVE_ADDR = 0x40;
//B1
const byte I2C_SLAVE_ADDR = 0x41;


int array_temp_selected[4] = {7, 7, 7, 7};

int cmd_num = 10;
int cmd_value = 5;

bool asSlave = false;
