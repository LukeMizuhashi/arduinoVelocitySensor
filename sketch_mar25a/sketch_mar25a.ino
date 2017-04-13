#include "Sensor.h"

Sensor sensor1(0,2,3,4,5,0.01);

void setup() {
    Serial.begin(9600);
    sensor1.initialize();
}

void loop() {
  unsigned long loopStartMicro = micros();
  
  sensor1.update(true);
  
  sensor1.waitForNextLoop(loopStartMicro);
}


