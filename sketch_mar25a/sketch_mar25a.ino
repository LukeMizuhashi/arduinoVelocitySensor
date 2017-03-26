#include "Sensor.h"

Sensor sensor1(A0,2,3,4,0.01);

void setup() {
    Serial.begin(9600);
    sensor1.cycleStatusLights();
}

void loop() {
  unsigned long loopStartMicro = micros();
  const double VOLTAGE = sensor1.update(true);
  sensor1.waitForNextLoop(loopStartMicro);
}


