#include <Wire.h>
#include "Temp_sensors.h"
#include "Burn_resistor_control.h"

const byte I2C_SLAVE_ADDR = 0x40;

int cmd_num = 10;
int cmd_value = 5;

bool asSlave = false;
