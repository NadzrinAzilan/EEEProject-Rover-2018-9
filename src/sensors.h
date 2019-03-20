#ifndef SENSORS_H
#define SENSORS_H

#include "pin.h"

void setupSensors();
unsigned int readMagneticSensors();
unsigned int readInfraredSensors();
unsigned int readAcousticSensors();
unsigned long readRadioSensors();

/* Configureation data */
const unsigned int UART_BPS = 600u;
const bool inverse_INFRARED = true;

/* Variables to store last known signal */
extern volatile unsigned int sensor_magnetic;
extern volatile unsigned int sensor_infrared;
extern volatile unsigned int sensor_acoustic;
extern volatile unsigned long sensor_radio;
#endif
