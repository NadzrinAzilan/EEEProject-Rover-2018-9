#ifndef ACTUATORS_H
#define ACTUATORS_H

#include "pin.h"

/* Utility function */
template<typename T> static inline T scale(T in, T MIN_IN, T MAX_IN, T MIN_OUT, T MAX_OUT) {
	return (((in-MIN_IN)%(MAX_IN-MIN_IN+1))/(MAX_IN-MIN_IN))*(MAX_OUT-MIN_OUT) + MIN_OUT;
}

/* range of values from the input */
const unsigned int MAX_INPUT_VALUE = 255u;
const unsigned int MIN_INPUT_VALUE = 0u;

/* Range of values to be fed in to the PWM pins */
const unsigned int HIGH_SPEED_PWM_VALUE = 255u;
const unsigned int LOW_SPEED_PWM_VALUE = 0u;

const unsigned long MICROSECONDS = 1000000;

void setup_PWM();

enum WHEEL_SIDE : bool{
	LEFT = false,
	RIGHT = true
};
enum WHEEL_DIRECTION : bool{
	FORWARD = true,
	BACKWARD = false
};

void wheel_changeAllSpeed(unsigned int);
void wheel_moveForward(WHEEL_SIDE);
void wheel_moveForwardWithPeriod(WHEEL_SIDE, unsigned int);
void wheel_moveBackward(WHEEL_SIDE);
void wheel_moveBackwardWithPeriod(WHEEL_SIDE, unsigned int);
void wheel_joystick(unsigned long);
void wheel_stop(WHEEL_SIDE);
void wheel_stop(bool, bool);

#endif
