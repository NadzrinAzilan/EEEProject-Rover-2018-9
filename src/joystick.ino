#include "joystick.h"
#include "actuators.h"

#define abs(X) (X>0?X:-1*X)
#define COS_ACCURACY 3u
#define PI_2 PI/2
#define PI_4 PI/4

/* Required for square root numerical calculation */
double sqrt_const[] = {100, 37000, 186.3538406, 2.599376225e-3, 1.756335287e-8, 3.955710106e-13};
const unsigned int RANGE = HIGH_SPEED_PWM_VALUE-LOW_SPEED_PWM_VALUE;

inline float fast_arctan(float x){
	return ((x>PI_2||(-x)>PI_2) ? PI_2*(x<0?-1:1) : x);
}

float fast_cos(float x){
	x *= (x<0?-1:1);
	while(x>2*PI) x -= 2*PI;
	if(x > PI) x = 2*PI - x;
	float y = 1, x_p = 1, fact = 1;
	for(int i = 2, j=1; i <= 2*(COS_ACCURACY+1); i+=2){
		j *= -1;
		fact *= i*(i-1);
		x_p *= x*x;
		y += j*x_p/fact;
	}
	return y;
}

double fast_sqrt(unsigned int x){
	double res = sqrt_const[2], x_term = 1;
	for(int i = 0; i < 3; i++){
		x_term *= (x-sqrt_const[1]);
		res += sqrt_const[i+3]*x_term;
	}
	if(res < 0) res *= -1;
	if(res < sqrt_const[0]) res = 0;
	return res;
}

float get_angle(int x, int y){
	float tmp = 0;
	if(x == 0 && y == 0) return 0;
	if(x==0 && y != 0){
		return float(PI_2*(y<0?-1:1));
	}
	else if (x != 0 && y==0){
		return float(x<0?PI:0);
	}
	else if(x > 0 && y > 0){
		return float(fast_arctan(float(y)/x));
	}
	else if (x>0 && y < 0){
		return float( -1* (PI + fast_arctan(float(y)/x)) );
	}
	else if (x<0 && y > 0){
		return float(PI + fast_arctan(float(y)/x));
	}
	else if (x<0 && y < 0){
		return float(-1*fast_arctan(float(y)/x));
	}
	else return float( ( (x==0?PI_2*(y<0?-1:1):fast_arctan(float(y)/x)) + ((y==0 && x < 0)?PI:0) ) );
}

inline unsigned int mag(int x, int y){
	return int(fast_sqrt(x*x+y*y));
}

void convertToWheelSpeed(int x,int y, int& l, int& r){
	/* Cap x and y to maximum range */
	if(abs(x) > HIGH_SPEED_PWM_VALUE) x = RANGE*abs(x);
	if((y<0?y*-1:y) > RANGE) y = RANGE*(y<0?-1:1);
	int rm = mag(x,y); if(rm > RANGE) rm = RANGE;
	float theta = get_angle(x,y);

	int l1 = int(rm*fast_cos(theta-PI_4));
	int r1 = -1*int(rm*fast_cos(theta+PI_4));

	if(l1 > int(RANGE) || (-l1) > int(RANGE)) l1 = RANGE * (l1<0?-1:1);
	if(r1 > int(RANGE) || (-r1) > int(RANGE)) r1 = RANGE * (r1<0?-1:1);

	l = l1; r = r1;
}


