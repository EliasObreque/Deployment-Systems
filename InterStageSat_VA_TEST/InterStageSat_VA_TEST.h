#include <Wire.h>
#include "Temp_sensors.h"
#include "Burn_resistor_control.h"
#include "PhotoDiodeFSS.h"

const byte I2C_SLAVE_ADDR_A = 0x40;
const byte I2C_SLAVE_ADDR_B = 0x41;

int array_temp_selected[num_temp_sensor] = {7, 7, 7, 7, 7, 7};

int cmd_num = 10;
int cmd_value = 5;

bool asSlave = false;
