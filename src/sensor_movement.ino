#include "sensor_movement.h"
#include "pin.h"
#include "time.h"

unsigned int sensor_callback_id = 0;

void setupSensorMotor(){
    pinMode(SENSOR_MOTOR_PWM_PIN, OUTPUT);
    pinMode(SENSOR_MOTOR_DIR_PIN, OUTPUT);
}

inline void sensor_m_cw(){
    sensor_m_move(false);
}

inline void sensor_m_ccw(){
    sensor_m_move(true);
}

void sensor_m_move(bool direction){
    if(sensor_callback_id > 0) unregisterTimer(sensor_callback_id);
    else {
        sensor_callback_id = registerTimerWithCallback(1*MICROSECONDS, sensor_m_stop, false);
        /* digitalWrite(SENSOR_MOTOR_DIR_PIN, direction); */ /* Not enough pin :( */
        diigtalWrite(SENSOR_MOTOR_PWM_PIN, true);
    }
}

void sensor_m_stop(){
    if(sensor_callback_id > 0) {
        /* unregisterTimer(sensor_callback_id, false); */
        sensor_callback_id = 0;
    }
    
    digitalWrite(SENSOR_MOTOR_DIR_PIN, false);
    digitalWrite(SENSOR_MOTOR_PWM_PIN, false);
}