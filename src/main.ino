#include "time.h"
#include "sensors.h"
#include "comms.h"
#include "actuators.h"

void setup(){
    blockMili(1000);
    if(Serial) Serial.end();
    Logln("Connection set.");
    
	resetLOG();
    setupTimer();
    setup_PWM();
    setupSensors();
    setupWiFi();
    setupHTTPServer();
    
    Logln();
    Logln("Start listening.");
    Logln();
}

void loop(){
    timerISR();
}

