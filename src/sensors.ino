#define SENSOR_CPP
#include "sensors.h"
#include "time.h"
#include "actuators.h"

volatile unsigned int  sensor_magnetic = 0; /* 0: No signal; 1: Downwards; 2: Upwards; 3: Hall probe/Arduino pin broken */
volatile unsigned int  sensor_infrared = 0; /* 0: No signal; 1: 353Hz; 2: 571Hz */
volatile unsigned int  sensor_acoustic = 0; /* 0: No signal; 1: Signal is detected */
volatile unsigned long sensor_radio    = 0; /* 0: No signal/match; otherwise: the 4 letter char in integer form with base 256 */

void setupSensors(){
	pinMode(SENSOR_MAGNETIC_PIN_UP, INPUT_PULLUP);
	pinMode(SENSOR_MAGNETIC_PIN_DOWN, INPUT_PULLUP);
	pinMode(SENSOR_ACOUSTIC_PIN, INPUT_PULLUP);
	pinMode(SENSOR_INFRARED_PIN, INPUT_PULLUP);
	pinMode(SENSOR_RADIO_PIN, INPUT_PULLUP);
}

unsigned int readMagneticSensors(){
	return sensor_magnetic = (analogRead(SENSOR_MAGNETIC_PIN_UP)>1000?1:0)*2 + (analogRead(SENSOR_MAGNETIC_PIN_DOWN)>1000?1:0);
}
unsigned int readAcousticSensors(){
	return sensor_magnetic = analogRead(SENSOR_ACOUSTIC_PIN)>1000?1:0;
}

unsigned int readInfraredSensors(){
	//353Hz, D+:90% -> D- period = (1/353)*1000000*0.9 us = 2550 us
	//571Hz, D-:95% -> D- period = (1/353)*1000000*0.9 us = 1664 us
	long time_micro = pulseIn(SENSOR_INFRARED_PIN, inverse_INFRARED, 6000); //timeout at 6ms (approx. 2550us * 2)
	if(time_micro > 2000) sensor_magnetic = 1; // 353Hz detected
	else if (time_micro > 1000) sensor_magnetic = 2; //571Hz detected
	else sensor_magnetic = 0; //Timeout reached, no detection
	return sensor_magnetic;
}

unsigned long readRadioSensors(){
	//Don't want the rover to be moving when sampling
	wheel_stop_all();

	unsigned long time_sample = 300000; //100'000 microseconds = 0.1 s
	char c[4] = {0}; unsigned int i = 0;
	digitalWrite(RADIO_FREQ_SEL_PIN,false);

	Serial.begin(UART_BPS, SERIAL_8N1);
	blockMicro(10); //let everyhing settles down
	while(Serial.available()) Serial.read(); //Clear buffer
	unsigned long t = micros();
	while(i < 4 && micros() - t < time_sample){
		if(Serial.available())
			c[i++] = Serial.read();
		if(i==4) break;
	}

	if(i<4) {
		digitalWrite(RADIO_FREQ_SEL_PIN,true);
		c[0] = c[1] = c[2] = c[3] = i = 0; //Clear buffer
		unsigned long t = micros();
		while(micros() - t < time_sample){
			if(Serial.available())
			c[i++] = Serial.read();
			if(i==4) break;
		}
	}

	//reset everything to default
	digitalWrite(RADIO_FREQ_SEL_PIN,false);

	//close Serial
	Serial.end();

	//return the results (can be '\0' if nothing detected)
	//format of output is 4 characters in base 256
	return sensor_radio = c[0]+c[1]*(256ul)+c[2]*(256ul*256ul)+c[3]*(256ul*256ul*256ul);
}