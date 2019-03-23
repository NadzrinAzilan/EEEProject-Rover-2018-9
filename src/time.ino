#include "time.h"

/* Global Variable */
const unsigned char TIMER_NUMBER = 20u;
bool timer_id_t[TIMER_NUMBER];
unsigned long timer_period[TIMER_NUMBER];
unsigned long timer_value[TIMER_NUMBER];
void (*timer_callback[TIMER_NUMBER])();
bool timer_repeat[TIMER_NUMBER];
unsigned long _micro = 0ul;
/* End Global Variable */

void setupTimer(){
    //Set up TIMER0 interrupt
}

/// Check all timer for expiry
void timerISR() {
    for(unsigned char i = 0; i < TIMER_NUMBER; i++)
        if(timer_id_t[i])
            if(micros() - timer_value[i] >= timer_period[i]) unregisterTimer(i);
}

void timer2ISR(){
    _micro++;
}

unsigned long micro(){return _micro;}

void blockMili(unsigned long x){
    unsigned long timeElapsed = micros();
    while(micros()-timeElapsed < x*1000) delay(x);
}
void blockMicro(unsigned long x){
    unsigned long timeElapsed = micros();
    while(micros()-timeElapsed < x) delayMicroseconds(x);
}

inline bool registeredTimer(unsigned char id){
    return timer_id_t[id];
}

void unregisterTimer(unsigned char id){
    unregisterTimer(id, true);  
}

void unregisterTimer(unsigned char id, bool call){
    if(!registeredTimer(id)) return;

    void (*tmp)() = timer_callback[id];
    if(!timer_repeat[id]){
        timer_id_t[id] = false;
        timer_value[id] = 0u;
        timer_period[id] = 0u;
        timer_callback[id] = nullptr;
        timer_repeat[id] = false;
    }
    if(tmp != nullptr && call) tmp();    
}

inline unsigned char getID(){
    for(unsigned char i = 1u; i <= TIMER_NUMBER; i++)
        if(!registeredTimer(i)) return i;
    return 255u;
}

///    x     : period of the timer in us
/// callback : function to be called when timer expired
/// Return   : the ID of the timer
unsigned char registerTimerWithCallback(unsigned long x, void(*callback)(), bool repeat){
    unsigned char id = registerTimer(x);
    if(id != 255 && id < TIMER_NUMBER){
        timer_callback[id] = callback;
        timer_repeat[id] = repeat;
    }
    return id;
}

///    x   : period of the timer in us
/// Return : the ID of the timer
unsigned char registerTimer(unsigned long x){
    unsigned char id = getID();
    if(id == 255u) //All timer are registered
        return 255u;
    
    timer_id_t[id] = true;
    timer_value[id] = micros();
    timer_period[id] = x;

    return id;
}

unsigned long getTimerElapsed(unsigned int id){
    if(!registeredTimer(id)) return 0;

    return micros() - timer_value[id];
}

