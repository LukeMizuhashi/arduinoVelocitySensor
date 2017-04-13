/*
  Sensor.h - Library for modeling light dependent velocity sensors.
  Created by Luke Mizuhashi, March 1, 2017.
*/

#include "Arduino.h"
#include "Sensor.h"

#include "Timer.h"

Sensor::Sensor(int inputPin,int lightSource,int redPinOut,int yellowPinOut,int greenPinOut,float ballDiameter)
{
  init(inputPin,redPinOut,yellowPinOut,greenPinOut,lightSource,ballDiameter,false);
}

Sensor::Sensor(int inputPin,int lightSource,int redPinOut,int yellowPinOut,int greenPinOut,float ballDiameter,boolean useMicroseconds)
{
  init(inputPin,redPinOut,yellowPinOut,greenPinOut,lightSource,ballDiameter,useMicroseconds);
}

void Sensor::init(int inputPin,int redPinOut,int yellowPinOut,int greenPinOut,int lightSource,float ballDiameter,boolean useMicroseconds)
{
  MAX_ANALOG_READ_RATE_MICRO = 100; // One read per 100 microseconds
  
  SENSOR_IN = inputPin;
  LIGHT_SOURCE = lightSource;
  RED_OUT = redPinOut;
  YELLOW_OUT = yellowPinOut;
  GREEN_OUT = greenPinOut;

  pinMode(LIGHT_SOURCE,OUTPUT);
  pinMode(GREEN_OUT,OUTPUT);
  pinMode(YELLOW_OUT,OUTPUT);
  pinMode(RED_OUT,OUTPUT);

  Timer timer(useMicroseconds);

  velocity = 0;

  BALL_DIAMETER = ballDiameter;

  inErrorState = false;
}

void Sensor::initialize() {
  digitalWrite(LIGHT_SOURCE,HIGH);
  
  digitalWrite(RED_OUT,HIGH);
  delay(500);
  digitalWrite(RED_OUT,LOW);
  
  digitalWrite(YELLOW_OUT,HIGH);
  minVoltage = 100;
  maxVoltage = 0;
  double voltage;
  for (short counter = 0; counter < 5000; counter++) {
    voltage = getVoltage();
    if (voltage < minVoltage ) {
      minVoltage = voltage;
    } else if (voltage > maxVoltage) {
      maxVoltage = voltage;
    }
  }
  TRIGGER_VOLTAGE = minVoltage * 0.98;
  delay(500);
  digitalWrite(YELLOW_OUT,LOW);

  digitalWrite(GREEN_OUT,HIGH);
  delay(500);
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
  if (inErrorState) {
    digitalWrite(RED_OUT,HIGH);
    digitalWrite(YELLOW_OUT,LOW);
    digitalWrite(GREEN_OUT,LOW);
  } else {
    if (beamBroken(voltage)) {
      digitalWrite(RED_OUT,LOW);
      digitalWrite(YELLOW_OUT,HIGH);
      digitalWrite(GREEN_OUT,LOW);
    } else {
      digitalWrite(RED_OUT,LOW);
      digitalWrite(YELLOW_OUT,LOW);
      digitalWrite(GREEN_OUT,HIGH);
    }
  }
}

boolean Sensor::beamBroken(double voltage) {
  return (voltage < TRIGGER_VOLTAGE);
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

//  message += " Maximum Voltage (V): ";
//  message += getMaxVoltage();

  message += " Trigger Voltage (V): ";
  message += TRIGGER_VOLTAGE;

//  message += " Timer (s): ";
//  message += timer.getTime();
//  
//  message += " Ball Diameter (m): ";
//  message += BALL_DIAMETER;
//
//  message += " Velocity (m/s): ";
//  message += velocity;
  
  Serial.println(message);
}


