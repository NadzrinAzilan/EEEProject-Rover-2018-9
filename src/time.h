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

const unsigned long MICROSECONDS = 1000000;
const unsigned long MILISECONDS = 1000;

#endif

