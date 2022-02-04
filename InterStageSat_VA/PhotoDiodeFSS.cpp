
#include "PhotoDiodeFSS.h"


void PhotoDiodeFSS::Mode(void){
	pinMode(PIN_DIODE_1, INPUT);
	pinMode(PIN_DIODE_2, INPUT);
	pinMode(PIN_DIODE_3, INPUT);
	pinMode(PIN_DIODE_4, INPUT);

}

void PhotoDiodeFSS::setDefault(){
	rotationT[0][0] = cos(gamma);
	rotationT[0][1] = sin(gamma);
	rotationT[1][0] = -sin(gamma);
	rotationT[1][1] = cos(gamma);
}

void PhotoDiodeFSS::read(){
	pd_measure[0] = analogRead(PIN_DIODE_1);
	pd_measure[1] = analogRead(PIN_DIODE_2);
	pd_measure[2] = analogRead(PIN_DIODE_3);
	pd_measure[3] = analogRead(PIN_DIODE_4);
}

int PhotoDiodeFSS::get_measure(void){
	return pd_measure;
}

void PhotoDiodeFSS::print(){
	for (int i=0; i < 4; i++){
		Serial.print(pd_measure[i]); Serial.print("\t");
	}
	Serial.print("\n");
}

void PhotoDiodeFSS::run_loop_for_fss(){
	if (flag_i2c_sample_fss == 1){
		read();
		calc_sun_position();
	}
	if (flag_i2c_get_fss == 1){
		get_sun_vector();
		flag_i2c_get_fss = 0;
	}
}

void PhotoDiodeFSS::calc_sun_position(){
	float A = float(pd_measure[2]);
	float B = float(pd_measure[3]);
	float C = float(pd_measure[1]);
	float D = float(pd_measure[0]);

	// Serial.print(A); Serial.print("-"); Serial.print(B); Serial.print("-");Serial.print(C); Serial.print("-");Serial.println(D);

	float den = (A + B + C + D);
	if (den != 0){
		float xd = (A + B - C -D) / den + x0;
		float yd = (A + D - C -B) / den + y0;
		sun_vector_d[0] = rotationT[0][0] * xd + rotationT[0][1] * yd;
		sun_vector_d[1] = rotationT[1][0] * xd + rotationT[1][1] * yd;
	
		phi = atan2(sun_vector_d[0], sun_vector_d[1]);
		theta = atan(sqrt(pow(sun_vector_d[0], 2) + pow(sun_vector_d[1], 2)) / h);

	} else {
		// Shadow zone
		sun_vector_d[0] = 2;
		sun_vector_d[1] = 2;

		phi = 0.0;
		theta = - M_PI;
	}

	float xfss = cos(theta);
	float yfss = sin(theta) * cos(phi);
	float zfss = sin(theta) * sin(phi);

	sun_vector_c[0] = xfss * 10000.0; // Scale
	sun_vector_c[1] = yfss * 10000.0;
	sun_vector_c[2] = zfss * 10000.0;
}

void PhotoDiodeFSS::get_sun_vector(){
	//for (int i = 0; i < 2; i++){
	//	Serial.print(sun_vector_d[i]); Serial.print("\t");
	//}
	//Serial.print("\n");

	for (int i = 0; i < 3; i++){
		Serial.print(sun_vector_c[i]); Serial.print("\t");
	}
	Serial.print("\n");
}

void PhotoDiodeFSS::cmd2FSS(int cmd_num){
	if (cmd_num == SAMPLE_SENSORS_FSS){     // 38
		flag_i2c_sample_fss = 1;
	}
	else if (cmd_num == GET_SUN_VECTOR_FSS){ // 39
		flag_i2c_get_fss = 1;
	}
	else if (cmd_num == STOP_SENSORS_FSS){ // 40
		flag_i2c_sample_fss = 0;
	}
}

void PhotoDiodeFSS::response_from_FSS(int cmd_num){
	if (cmd_num == SAMPLE_SENSORS_FSS){  // 38
		Wire.write(0);
	}
	else if (cmd_num == GET_SUN_VECTOR_FSS){ // 39
		get_sun_vector();
		Wire.write((byte *) sun_vector_c, sizeof(sun_vector_c));
	}
	else if (cmd_num == STOP_SENSORS_FSS){ // 40
		Wire.write(0);
	}
}