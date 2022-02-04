#include "Arduino.h"
#include <math.h>
#include <Wire.h>

#define PIN_DIODE_1 A0 // Pos: -+
#define PIN_DIODE_2 A1 // Pos: ++
#define PIN_DIODE_3 A2 // Pos: -+
#define PIN_DIODE_4 A3 // Pos: +-

#define SAMPLE_SENSORS_FSS 38
#define GET_SUN_VECTOR_FSS 39
#define STOP_SENSORS_FSS 40


class PhotoDiodeFSS
{
private:
	float gamma = 0.0;
	float x0 = 0.0;
	float y0 = 0.0;
	float h = 1.0;
	float phi;
	float theta;
	float rad2deg = 180.0 / M_PI;
	float deg2rad = 1 / rad2deg;
	int flag_i2c_sample_fss = 0;
	int flag_i2c_get_fss = 0;

public:
	// Public Members
	float sun_vector_c[3] = {-1.0, -1.0, -1.0};
	int pd_measure[4];
	float sun_vector_d[2];
	float rotationT[2][2] = {{0.0, 0.0}, {0.0, 0.0}};

	// Constructor
	PhotoDiodeFSS(){};

	// methods
	void Mode(void);
	void setDefault();
	void read();
	int get_measure(void);
	void calc_sun_vector();
	void calc_sun_position();
	void get_sun_vector();
	void run_loop_for_fss();
	void print(void);
	void cmd2FSS(int cmd_num);
	void response_from_FSS(int cmd_num);
};
