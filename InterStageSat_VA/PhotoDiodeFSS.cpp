
#include "PhotoDiodeFSS.h"


void PhotoDiodeFSS::Mode(void){
	pinMode(PIN_DIODE_1, INPUT);
	pinMode(PIN_DIODE_2, INPUT);
	pinMode(PIN_DIODE_3, INPUT);
	pinMode(PIN_DIODE_4, INPUT);

}

void PhotoDiodeFSS::setDefault(){
	
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
