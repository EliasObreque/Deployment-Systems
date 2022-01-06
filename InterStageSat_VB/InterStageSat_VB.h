#include <Wire.h>
#include "Burn_resistor_control.h"
#include "PhotoDiodeFSS.h"

const byte I2C_SLAVE_ADDR_A = 0x40;
const byte I2C_SLAVE_ADDR_B = 0x41;

int cmd_num = 10;
int cmd_value = 5;

bool asSlave = false;