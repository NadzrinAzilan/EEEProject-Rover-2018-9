#include "sensors.h"
#include "time.h"
#include "actuators.h"
#include "pin.h"

volatile unsigned int  sensor_magnetic = 0; /* 0: No signal; 1: Downwards; 2: Upwards; 3: Hall probe/Arduino pin broken */
volatile unsigned int  sensor_infrared = 0; /* 0: No signal; 1: 353Hz; 2: 571Hz */
volatile unsigned int  sensor_acoustic = 0; /* 0: No signal; 1: Signal is detected */
volatile unsigned long sensor_radio    = 0; /* 0: No signal/match; otherwise: the 4 letter char in integer form with base 256 */

void setupSensors(){
	pinMode(SENSOR_MAGNETIC_PIN_UP, INPUT_PULLUP);
	pinMode(SENSOR_MAGNETIC_PIN_DOWN, INPUT_PULLUP);
	pinMode(SENSOR_ACOUSTIC_PIN, INPUT_PULLUP);
	pinMode(SENSOR_INFRARED_PIN, INPUT_PULLUP);
	/* pinMode(SENSOR_RADIO_PIN, INPUT_PULLUP); */
	pinMode(RADIO_FREQ_SEL_PIN, OUTPUT);
}

unsigned int readMagneticSensors(){
	return sensor_magnetic = (analogRead(SENSOR_MAGNETIC_PIN_UP)>1000?1:0)*2 + (analogRead(SENSOR_MAGNETIC_PIN_DOWN)>1000?1:0);
}
unsigned int readAcousticSensors(){
	return sensor_acoustic = analogRead(SENSOR_ACOUSTIC_PIN)>1000?1:0;
}

unsigned int readInfraredSensors(){
	/* 353Hz, D+:90% -> D- period = (1/353)*1000000*0.9 us = 2550 us */
	/* 571Hz, D-:95% -> D- period = (1/353)*1000000*0.9 us = 1664 us */
	long time_micro = pulseIn(SENSOR_INFRARED_PIN, inverse_INFRARED, 6000); /* timeout at 6ms (approx. 2550us * 2) */
	if(time_micro > 2000) sensor_magnetic = 1; /* 353Hz detected */
	else if (time_micro > 1000) sensor_magnetic = 2; /* 571Hz detected */
	else sensor_magnetic = 0; /* Timeout reached, no detection */
	return sensor_magnetic;
}

unsigned long readRadioSensors(){
	/* Prevent rover from moving when sampling */
	wheel_stop(true, true);

	char c[4] = {0}; unsigned int i = 0;
	digitalWrite(RADIO_FREQ_SEL_PIN,LOW);

	/* Close existing Serial connection and wait until everything settle down */
 	if(Serial) Serial.end();
	blockMili(10);

	/* Set baud rate, and UART format (1 start bit, 8 bit data, no parity, 1 stop bit) */
	Serial.begin(UART_BPS, SERIAL_8N1);
	while(!Serial) blockMicro(100); /* Ensure port is completely initialised */
	while(Serial.available()) Serial.read(); /* Clear buffer */
	unsigned long t = micros();
	while(i < 4 && micros() - t < SAMPLING_TIME){
		if(Serial.available())
			c[i++] = Serial.read();
	}

	if(i<4) { /* choose another frequency if no letter found */
		digitalWrite(RADIO_FREQ_SEL_PIN,HIGH);
		c[0] = c[1] = c[2] = c[3] = i = 0; /* Clear buffer */
		t = micros();
		while(i < 4 && micros() - t < SAMPLING_TIME){
			if(Serial.available())
				c[i++] = Serial.read();
		}
	}

	/* reset everything to default */
	digitalWrite(RADIO_FREQ_SEL_PIN, LOW);

	/* close Serial */
	Serial.end();

	/*return the results (can be '\0' if nothing detected)
	  format of output is 4 characters in base 256 (2^8) */
	return sensor_radio = c[0]+c[1]*(256ul)+c[2]*(256ul*256ul)+c[3]*(256ul*256ul*256ul);
}
