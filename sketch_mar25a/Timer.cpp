/*
  Timer.h - Library for modeling timers ( i.e. stop watches ).
  Created by Luke Mizuhashi, March 1, 2017.
*/

#include "Arduino.h"
#include "Timer.h"

Timer::Timer() {
  USE_MICROS = false;
  reset();
}

Timer::Timer(boolean useMicroSeconds) {
  USE_MICROS = useMicroSeconds;
  reset();
}

void Timer::reset() {
  IS_RUNNING = false;
  startTime = -1;
  stopTime = -1;
}

float Timer::start() {
  float result;
  if (isRunning()) {
    reset();
    result = -1;
  } else {
    IS_RUNNING = true;
    startTime = getSystemTime();
    result = 0;
  }
  return result;
}

float Timer::stop() {
  float result;
  if (isRunning()) {
    stopTime = getSystemTime();
    IS_RUNNING = false;
    result = getTime(stopTime);
  } else {
    reset();
    result = -1;
  }
  return result;
}

float Timer::getTime() {
  float result;
  if (isRunning()) {
    unsigned long lapTime = getSystemTime();
    result = getTime(lapTime);
  } else {
    result = -1;
  }
  return result;
}

float Timer::getTime(unsigned long lapTime) {
  float result;
  if ( lapTime >= 0 && startTime >= 0 && lapTime > startTime) {
    result = lapTime - startTime;
  } else {
    result = -1;
  }
  if (USE_MICROS) {
    result /= 1000000;
  } else {
    result /= 1000;
  }
  return result; 
}

unsigned long Timer::getSystemTime() {
  unsigned long result;
  if (USE_MICROS) {
    result = micros();
  } else {
    result = millis();
  }
  return result;
}

boolean Timer::isRunning() {
  return IS_RUNNING;
}

