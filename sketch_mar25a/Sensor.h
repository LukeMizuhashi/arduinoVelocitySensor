/*
  Sensor.h - Library for modeling light dependent velocity sensors.
  Created by Luke Mizuhashi, March 1, 2017.
*/

#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include "Sensor.h"

#include "Timer.h"

class Sensor
{
  public:
    Sensor(int inputPin,int redPinOut,int yellowPinOut,int greenPinOut,int light,float ballDiameter);
    Sensor(int inputPin,int redPinOut,int yellowPinOut,int greenPinOut,int light,float ballDiameter,boolean useMicroseconds);

    void primeSensor();
    
    double update();
    double update(boolean writeToLog);
    
    void waitForNextLoop(unsigned long loopStartMicro);
    
  private:
    void init(int inputPin,int redPinOut,int yellowPinOut,int greenPinOut,int light,float ballDiameter,boolean useMicroseconds);

    double getVoltage();
    double getMinVoltage();
    double getMaxVoltage();

    boolean beamBroken(double voltage);
    
    void updateMin(double voltage);
    void updateMax(double voltage);

    void updateView(double voltage);

    void updateVelocity(double voltage);
    
    void log(double voltage);
    
    boolean inErrorState;
    
    int SENSOR_IN;

    int RED_OUT;
    int YELLOW_OUT;
    int GREEN_OUT;

    int LIGHT;
        
    short MAX_ANALOG_READ_RATE_MICRO;

    double minVoltage;
    double maxVoltage;
    double TRIGGER_VOLTAGE_DROP;

    Timer timer;

    float velocity;

    float BALL_DIAMETER;
};

#endif
