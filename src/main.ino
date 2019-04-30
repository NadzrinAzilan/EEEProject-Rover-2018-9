#include "time.h"
#include "sensors.h"
#include "comms.h"
#include "actuators.h"
#include "debug.h"

void setup(){
    blockMili(1000);
    if(Serial) Serial.end();
    Logln("Connection set.");
    
    setupTimer();
    setup_PWM();
    setupSensors();
    if(setupWiFi()) setupHTTPServer();
    else blinkLedError(); //Infinite LED blink loop. Require reset button to be pressed/restart Arduino.
    
    Logln("\nStart listening.\n");
}

void loop(){
    timerISR();
}

void blinkLedError(){
    while(true){
        digitalWrite(LED_PIN,true);
        blockMili(200);
        digitalWrite(LED_PIN,false);
        blockMili(200);
    }
}