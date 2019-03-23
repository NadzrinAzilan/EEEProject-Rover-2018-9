#include "time.h"
#include "sensors.h"
#include "comms.h"
#include "actuators.h"
#include "debug.h"

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
    
    Logln("\nStart listening.\n");
}

void loop(){
    timerISR();
}


