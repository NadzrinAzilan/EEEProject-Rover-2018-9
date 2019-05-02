#include "time.h"
#include "sensors.h"
#include "comms.h"
#include "actuators.h"
#include "debug.h"
#include "sensor_movement.h"
#include "pin.h"

void setup(){
    blockMili(1000);
    if(Serial) Serial.end();
    Logln("Connection set.");

    pinMode(LED_PIN, OUTPUT);
    setupTimer();
    setup_PWM();
    setupSensors();
    setupSensorMotor();
    //if(setupWiFi()) setupHTTPServer();
    //else blinkLedError(); /* Infinite LED blink loop. Require reset button to be pressed/restart Arduino. */
    blinkLedError();
    
    Logln("\nStart listening.\n");
}

void loop(){
    timerISR();
}

void blinkLedError(){
    while(true){
        digitalWrite(LED_PIN,HIGH);
        blockMili(200);
        digitalWrite(LED_PIN,LOW);
        blockMili(200);
    }
}
