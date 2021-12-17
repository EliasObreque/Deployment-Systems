#include "Arduino.h"

#define PIN_DIODE_1 A0 // Pos: -+
#define PIN_DIODE_2 A1 // Pos: ++
#define PIN_DIODE_3 A2 // Pos: -+
#define PIN_DIODE_4 A3 // Pos: +-

void setup_photodiodo();


class PhotoDiodeFSS
{
public:
	// Public Members
	float sun_vector_b[3];
	int pd_measure[4];

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
	void print(void);
	
};
