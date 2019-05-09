#include "pin.h"
#include "actuators.h"
#include "joystick.h"

/* Macro functions */
#define GetWheel(w) (w?MOTOR_1_PWM_PIN:MOTOR_2_PWM_PIN)
#define GetWheelDir(w) (w?MOTOR_1_DIR_PIN:MOTOR_2_DIR_PIN)
#define GetSpeed(w) wheelSpeed[w]

bool stopWheel[2] = {false,false};
bool isMovingWheel[2] = {0,0}; 
volatile unsigned int wheelSpeed[2] = {LOW_SPEED_PWM_VALUE,LOW_SPEED_PWM_VALUE};

void setup_PWM(){
	pinMode(MOTOR_1_PWM_PIN, OUTPUT);
	pinMode(MOTOR_1_DIR_PIN, OUTPUT);
	pinMode(MOTOR_2_PWM_PIN, OUTPUT);
	pinMode(MOTOR_2_DIR_PIN, OUTPUT);

	set_wheel_direction(WHEEL_SIDE::LEFT, WHEEL_DIRECTION::FORWARD);
	set_wheel_direction(WHEEL_SIDE::RIGHT, WHEEL_DIRECTION::FORWARD);
	wheel_stop(true, true);
}

inline unsigned int scale_wheel_speed(unsigned int x){
	return scale(x, MIN_INPUT_VALUE, MAX_INPUT_VALUE, LOW_SPEED_PWM_VALUE, HIGH_SPEED_PWM_VALUE);
}
void set_wheel_direction(WHEEL_SIDE w, WHEEL_DIRECTION state){
	digitalWrite(GetWheelDir(w), HL(state));
}
void set_wheel_direction(WHEEL_DIRECTION left, WHEEL_DIRECTION right){
	digitalWrite(GetWheelDir(WHEEL_SIDE::LEFT) , HL(left) );
	digitalWrite(GetWheelDir(WHEEL_SIDE::RIGHT), HL(right));
}
void wheel_move(WHEEL_SIDE w){
	isMovingWheel[WHEEL_SIDE::LEFT ] = w==WHEEL_SIDE::LEFT ;
	isMovingWheel[WHEEL_SIDE::RIGHT] = w==WHEEL_SIDE::RIGHT;
	wheel_move();
}
void wheel_move(bool left, bool right){
	isMovingWheel[WHEEL_SIDE::LEFT]  = left;
	isMovingWheel[WHEEL_SIDE::RIGHT] = right;
	wheel_move();
}
void wheel_move(){
	analogWrite(GetWheel(WHEEL_SIDE::LEFT) , isMovingWheel[WHEEL_SIDE::LEFT ]?scale_wheel_speed(GetSpeed(WHEEL_SIDE::LEFT) ):0);
	analogWrite(GetWheel(WHEEL_SIDE::RIGHT), isMovingWheel[WHEEL_SIDE::RIGHT]?scale_wheel_speed(GetSpeed(WHEEL_SIDE::RIGHT)):0);
}



/* Basic functions to move the wheel */
void wheel_moveForward(WHEEL_SIDE w){
	set_wheel_direction(w, WHEEL_DIRECTION::FORWARD);
	wheel_move(w);
}
void wheel_moveForwardWithPeriod(WHEEL_SIDE w, unsigned int x){
	set_wheel_direction(w,WHEEL_DIRECTION::FORWARD);
	wheel_move(w);
	stopWheel[w] = true;
	registerTimerWithCallback(x*MICROSECONDS, wheel_move_stop_callback, false);
}
void wheel_moveBackward(WHEEL_SIDE w){
	set_wheel_direction(w,WHEEL_DIRECTION::BACKWARD);
	wheel_move(w,GetSpeed(w));
}
void wheel_moveBackwardWithPeriod(WHEEL_SIDE w, unsigned int x){
	set_wheel_direction(w,WHEEL_DIRECTION::BACKWARD);
	wheel_move(w,GetSpeed(w));

	stopWheel[w] = true;
	registerTimerWithCallback(x*MICROSECONDS, wheel_move_stop_callback, false);
}
/* Function to stop the wheel after a period of time passed */
void wheel_move_stop_callback(){
  wheel_stop(stopWheel[0], stopWheel[1]);
  stopWheel[0] = stopWheel[1] = false;
}
void wheel_change_speed(unsigned int xl, unsigned int xr){
	xl = scale(xl, MIN_INPUT_VALUE, MAX_INPUT_VALUE, LOW_SPEED_PWM_VALUE, HIGH_SPEED_PWM_VALUE);
	xr = scale(xr, MIN_INPUT_VALUE, MAX_INPUT_VALUE, LOW_SPEED_PWM_VALUE, HIGH_SPEED_PWM_VALUE);
	/*
		x now ranges from LOW_SPEED_PWM_VALUE to HIGH_SPEED_PWM_VALUE
		LOW_SPEED_PWM_VALUE to HIGH_SPEED_PWM_VALUE  : change the speed to this new value
		HIGH_SPEED_PWM_VALUE + 1                     : do not change the current speed
	*/

	/* Assign speed to the wheel */
	GetSpeed(WHEEL_SIDE::LEFT ) = xl;
	GetSpeed(WHEEL_SIDE::RIGHT) = xr;

	wheel_move();
}

/* Functions related to joystick operation */
void wheel_joystick(unsigned long n){
	int x_dir = (n/10000000)    >0 ?-1:1,
		y_dir = (n/1000000 )%10 >0 ?-1:1;
		
	/* remove direction digits and get last 6 digits */
	n %= 1000000;
	
	/* Combine the signs to the extracted values */
	int x_mag = x_dir*(n/1000),
		y_mag = y_dir*(n%1000);

	int l = 0, r = 0;
	convertToWheelSpeed(x_mag, y_mag, l, r);

	WHEEL_DIRECTION l_dir = (l<0?WHEEL_DIRECTION::BACKWARD:WHEEL_DIRECTION::FORWARD),
					r_dir = (r<0?WHEEL_DIRECTION::BACKWARD:WHEEL_DIRECTION::FORWARD);

	/* Get the absolute speed (without negative sign) */
	l *= (l<0?-1:1); r *= (r<0?-1:1);
	
	/* Reset all speed to 0 */
	wheel_stop(true, true);
	
	/* Chenge the direction and speed to new values */
	set_wheel_direction(WHEEL_SIDE::LEFT, l_dir);
	set_wheel_direction(WHEEL_SIDE::RIGHT, r_dir);
	wheel_change_speed(WHEEL_SIDE::LEFT, l);
	wheel_change_speed(WHEEL_SIDE::RIGHT, r);
	
	/* Move the wheels as with the new speed */
	wheel_move(true, true);
}


/* Functions that handle stopping the wheels */
void wheel_stop(WHEEL_SIDE w){
	wheel_stop(w==WHEEL_SIDE::LEFT, w==WHEEL_SIDE::RIGHT);
}
void wheel_stop(bool left, bool right){
	isMovingWheel[WHEEL_SIDE::LEFT ] = !left;
	isMovingWheel[WHEEL_SIDE::RIGHT] = !right;
	wheel_move();
}
