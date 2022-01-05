
#include <Wire.h>

#define READ_SW_FACE 33
#define BURN_FACE 34
#define SET_BURN 35

#define pin_sw_x_plus 8 
#define pin_sw_x_minus 4 
#define pin_sw_y_plus 7 
#define pin_sw_y_minus 2 

#define pin_burn_x_plus 3
#define pin_burn_x_minus 6
#define pin_burn_y_plus 5
#define pin_burn_y_minus 9

void read_sw_state(int face);
void set_burn_config(int cmd_num);
void activate_resistor(int face);
void setup_burning();
void run_loop_for_burn_resistor();
void cmd2Burn(int cmd_num, int cmd_value);
void response_from_Burn(int cmd_num, int cmd_value);