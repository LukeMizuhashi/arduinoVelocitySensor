const short LDR_PIN = A0;
const unsigned short SERIAL_BAUD_RATE = 9600;
const int VOLTAGE_IN_VOLTS = 5;

const unsigned long MAX_TIME_IN_MILLI = 4200000;
const unsigned short CLOCK_TOLERANCE_IN_MILLI = 500;

const unsigned long MAX_LONG = 4294967295;
const unsigned short MAX_ANALOG_READ_VALUE = 1023;
const unsigned short MAX_ANALOG_READ_RATE_IN_MICRO = 100;

const unsigned short MIN_SAMPLE_COUNT = 10;
unsigned long sampleCount;
short averageVoltage;
double standardDeviation;

void initializeData() {
  sampleCount = 0;
  averageVoltage = 0;
  standardDeviation = 0;
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  initializeData();
}

void loop() {
  unsigned long loopStartTime = micros();
  // checkForClockOverRun();
  
  short CURRENT_VOLTAGE = getCurrentVoltage(LDR_PIN,VOLTAGE_IN_VOLTS);
  sampleCount++;
  averageVoltage = (averageVoltage + CURRENT_VOLTAGE) / 2;

  // if (sampleCount >= MIN_SAMPLE_COUNT) {
    // abs(CURRENT_VOLTAGE - averageVoltage);
    // standardDeviation = sqrt(pow(CURRENT_VOLTAGE - averageVoltage,2)/(sampleCount - 1));
  // }
  
  // String message = "Average: " + averageVoltage;
  // message += "   Voltage Now: " + CURRENT_VOLTAGE;
  // message += "   Standard Deviation: ";
  // message += standardDeviation;
  Serial.println(CURRENT_VOLTAGE);
  
  unsigned long loopEndTime = micros();
  delayIfRequired(loopStartTime,loopEndTime);
}

short getCurrentVoltage(short pin,short powerSupplyVoltage) {
  unsigned short sensorValue = analogRead(pin);
  float voltage = sensorValue * ( powerSupplyVoltage / MAX_ANALOG_READ_VALUE );
  return voltage;
}

void delayIfRequired(unsigned long loopStartTime,unsigned long loopEndTime) {
  if ( loopEndTime - loopStartTime < MAX_ANALOG_READ_RATE_IN_MICRO ) {
    delay(MAX_ANALOG_READ_RATE_IN_MICRO - loopEndTime - loopStartTime);
  }
}

void checkForClockOverRun() {
  if ( MAX_TIME_IN_MILLI - millis() <= CLOCK_TOLERANCE_IN_MILLI ) {
    initializeData();
    while (millis() > MAX_TIME_IN_MILLI / 2) {
      // Do nothing.
    }
  }
}

