#include "Sensor.h"

const float BALL_DIAMETER = 0.01;

Sensor sensor1(A0,3,4,5,2,BALL_DIAMETER);
Sensor sensor2(A1,7,8,9,6,BALL_DIAMETER);
Sensor sensor3(A2,11,12,13,10,BALL_DIAMETER);


void setup() {
    Serial.begin(9600);
    sensor1.primeSensor();
    sensor2.primeSensor();
    sensor3.primeSensor();
}

void loop() {
  unsigned long loopStartMicro = micros();

  const boolean WRITE_TO_LOG = true;
  sensor1.update(!WRITE_TO_LOG);
  sensor2.update(!WRITE_TO_LOG);
  sensor3.update(WRITE_TO_LOG);
  
  sensor1.waitForNextLoop(loopStartMicro);
}


