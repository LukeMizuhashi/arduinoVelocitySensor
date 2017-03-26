/*
  Sensor.h - Library for modeling light dependent velocity sensors.
  Created by Luke Mizuhashi, March 1, 2017.
*/

#include "Arduino.h"
#include "Sensor.h"

#include "Timer.h"

Sensor::Sensor(int inputPin,int redPinOut,int greenPinOut,int bluePinOut,float ballDiameter)
{
  init(inputPin,redPinOut,greenPinOut,bluePinOut,ballDiameter,false);
}

Sensor::Sensor(int inputPin,int redPinOut,int greenPinOut,int bluePinOut,float ballDiameter,boolean useMicroseconds)
{
  init(inputPin,redPinOut,greenPinOut,bluePinOut,ballDiameter,useMicroseconds);
}

void Sensor::init(int inputPin,int redPinOut,int greenPinOut,int bluePinOut,float ballDiameter,boolean useMicroseconds)
{
  MAX_ANALOG_READ_RATE_MICRO = 100; // One read per 100 microseconds
  TRIGGER_VOLTAGE_DROP = 0.03;
  
  SENSOR_IN = inputPin;
  RED_OUT = redPinOut;
  GREEN_OUT = greenPinOut;
  BLUE_OUT = bluePinOut;
  
  pinMode(BLUE_OUT,OUTPUT);
  pinMode(GREEN_OUT,OUTPUT);
  pinMode(RED_OUT,OUTPUT);

  Timer timer(useMicroseconds);

  velocity = 0;

  BALL_DIAMETER = ballDiameter;

  inErrorState = false;
}

double Sensor::update(boolean writeToLog)
{
  const double VOLTAGE = update();
  if (writeToLog) {
    log(VOLTAGE);
  }
  return VOLTAGE;
}

double Sensor::update()
{
  const double VOLTAGE = getVoltage();
  updateMin(VOLTAGE);
  updateMax(VOLTAGE);
  updateVelocity(VOLTAGE);
  updateView(VOLTAGE);
  return VOLTAGE;
}

double Sensor::getVoltage() {
    float sensorValue = analogRead(SENSOR_IN);
    return sensorValue * ( 5.0 / 1023.0);
}

void Sensor::updateMin(double voltage) {
  if (!minVoltage || voltage < minVoltage) {
    minVoltage = voltage;
  }
}

void Sensor::updateMax(double voltage) {
  if (!maxVoltage || voltage > maxVoltage) {
    maxVoltage = voltage;
  }
}

void Sensor::updateView(double voltage) {
  updateBeamBrokenView(voltage);
  updateStateView();
}

void Sensor::updateBeamBrokenView(double voltage) {
  if (beamBroken(voltage)) {
    digitalWrite(BLUE_OUT,HIGH);
  } else {
    digitalWrite(BLUE_OUT,LOW);
  }
}

void Sensor::updateStateView() {
  if (inErrorState) {
    digitalWrite(RED_OUT,HIGH);
    digitalWrite(GREEN_OUT,LOW);
  } else {
    digitalWrite(RED_OUT,LOW);
    digitalWrite(GREEN_OUT,HIGH);
  }
}

boolean Sensor::beamBroken(double voltage) {
  double triggerLevel = maxVoltage - TRIGGER_VOLTAGE_DROP;
  return (voltage < triggerLevel);
}

double Sensor::getMinVoltage() {
  return minVoltage;
}

double Sensor::getMaxVoltage() {
  return maxVoltage;
}

void Sensor::waitForNextLoop(unsigned long loopStartMicro) {
  unsigned long loopStopMicro = micros();
  unsigned long timeSpentThisLoop = loopStopMicro - loopStartMicro;
  if (timeSpentThisLoop < MAX_ANALOG_READ_RATE_MICRO) {
    delayMicroseconds(MAX_ANALOG_READ_RATE_MICRO - timeSpentThisLoop);
  }
}

void Sensor::updateVelocity(double voltage) {
  if (timer.isRunning()) {
    if (beamBroken(voltage)) {
      velocity = 0;
    } else {
      velocity = BALL_DIAMETER / timer.stop();
      timer.reset();
    }
  } else {
    if (beamBroken(voltage)) {
      timer.reset();
      timer.start();
      velocity = 0;
    } else {
      timer.reset();
      velocity = 0;
    }
  }
}

void Sensor::log(double voltage) {
  String message = " Voltage Now (V): ";
  message += voltage;

  message += " Minimum Voltage (V): ";
  message += getMinVoltage();

  message += " Maximum Voltage (V): ";
  message += getMaxVoltage();

  message += " Timer (s): ";
  message += timer.getTime();
  
  message += " Ball Diameter (m): ";
  message += BALL_DIAMETER;

  message += " Velocity (m/s): ";
  message += velocity;
  
  Serial.println(message);
}


