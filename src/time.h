#ifndef TIME_H
#define TIME_H

void setupTimer();
unsigned char registerTimerWithCallback(unsigned long, void(*)(), bool);
unsigned char registerTimer(unsigned long);
unsigned long getTimerElapsed(unsigned int);
void unregisterTimer(unsigned char, bool);
void unregisterTimer(unsigned int);
void blockMicro(unsigned long);
void blockMili(unsigned long);
void timerISR();
void timer2ISR();
unsigned long micro();

#endif

