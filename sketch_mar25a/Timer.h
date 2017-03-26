/*
  Timer.h - Library for modeling timers ( i.e. stop watches ).
  Created by Luke Mizuhashi, March 1, 2017.
*/

#ifndef Timer_h
#define Timer_h

#include "Arduino.h"

class Timer
{
  public:
    Timer();
    Timer(boolean useMicroSeconds);
    
    float start();
    float stop();
    float getTime();
    void reset();
    boolean isRunning();
    
  private:
    float getTime(unsigned long lapTime);
    unsigned long getSystemTime();
    
    boolean USE_MICROS;
    boolean IS_RUNNING;
    
    unsigned long startTime;
    unsigned long stopTime;
};

#endif
