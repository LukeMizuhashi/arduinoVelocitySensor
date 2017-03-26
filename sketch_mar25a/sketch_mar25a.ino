/*

*/

const int SENSOR_IN = A0;
const short MAX_ANALOG_READ_MICRO = 100; // One read per 100 microseconds
boolean beamBroken;

const int BLUE_OUT = 2;
const int GREEN_OUT = 3;
const int RED_OUT = 4;

double minVoltage;
double maxVoltage;
const double MAX_DOUBLE_TRIGGER_VOLTS = 0.02;

void setup() {
    Serial.begin(9600);
    pinMode(BLUE_OUT,OUTPUT);
    pinMode(GREEN_OUT,OUTPUT);
    pinMode(RED_OUT,OUTPUT);
}

void loop() {
    long loopStartMicro = micros();
    
    const float VOLTAGE = getVoltage(SENSOR_IN);
    updateMin(VOLTAGE);
    updateMax(VOLTAGE);
    updateState(VOLTAGE);
    updateView();

    writeToSerial(VOLTAGE);
    waitForNextLoop(loopStartMicro);
}

void updateState(double voltage) {
  double triggerLevel = maxVoltage - MAX_DOUBLE_TRIGGER_VOLTS;
  beamBroken = (voltage < triggerLevel);
}

void updateView() {
  if (beamBroken) {
    digitalWrite(BLUE_OUT,HIGH);
  } else {
    digitalWrite(BLUE_OUT,LOW);
  }
}

void writeToSerial(double voltage) {
  String message = " Voltage Now: ";
  message += voltage;

  message += " Minimum Voltage: ";
  message += minVoltage;

  message += " Maximum Voltage: ";
  message += maxVoltage;
  
  Serial.println(message);
}

void waitForNextLoop(long loopStartMicro) {
  long loopStopMicro = micros();
  long timeSpentThisLoop = loopStopMicro - loopStartMicro;
  if (timeSpentThisLoop < MAX_ANALOG_READ_MICRO) {
    delay(MAX_ANALOG_READ_MICRO - timeSpentThisLoop * 1000);
  }
}

float getVoltage(int pin) {
    float sensorValue = analogRead(pin);
    return sensorValue * ( 5.0 / 1023.0);
}

void updateMin(float voltage) {
  if (!minVoltage) {
    minVoltage = voltage;
  }
  if (voltage < minVoltage) {
    minVoltage = voltage;
  }
}

void updateMax(float voltage) {
  if (!maxVoltage) {
    maxVoltage = voltage;
  }
  if (voltage > maxVoltage) {
    maxVoltage = voltage;
  }
}

