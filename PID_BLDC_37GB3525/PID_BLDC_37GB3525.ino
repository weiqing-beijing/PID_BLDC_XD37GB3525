#include <PID_v1.h>
#define GearedWheelSpeed(x) (((x/10.0)/6.0) * 60.0 )  //the sensor reading is (((total no. of signals per second / gear ratio) / no. of signal per round of motor) * no. of second per minute) 

double Setpoint, Input, Output;
double aggKp = 5, aggKi = 0.2, aggKd = 1;
double consKp = 1, consKi = 0.05, consKd = 0.25;
const int SampleTime = 100; //100ms PID sample time as default

PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

const byte analogOutPin = 9; // Analog output pin that the motor is attached to
const byte speedInterruptReadPin = 7; //use digitalpin7/INT.4 as interrupt pin to check speed (https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)
byte outputValue = 0; // value output to the PWM (analog out)

volatile double counter, speedHz = 0;

void speedReading(void) {
  counter++;
  speedHz++;
}

void setup() {
  Input = counter;
  Setpoint = 60;

  //turn the PID on
  myPID.SetOutputLimits(30, 255);
  myPID.SetSampleTime(SampleTime);
  myPID.SetMode(AUTOMATIC);

  pinMode(analogOutPin, OUTPUT);
  pinMode(speedInterruptReadPin, INPUT_PULLUP);
  analogWrite(analogOutPin, Setpoint);
  attachInterrupt(digitalPinToInterrupt(speedInterruptReadPin), speedReading, FALLING);
  Serial.begin(9600);
}

void loop() {
  String inString;

  //use Serial Monitor to input the targeted RPM. Use RPM/10 as the speed measurement is 100ms while the calculation uses 'per second' data
  //For example, input 20 equals to targeted RPM at 200
  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      inString += (char)inChar;
      outputValue = inString.toInt();
    }
  }
  Setpoint = outputValue;//PID target
  
  static uint32_t sensorReadTimer = millis();
  if ((millis() - sensorReadTimer) >= 100) { //check sensor reading for every 100ms.
    sensorReadTimer = millis();
    Input = counter; //PID input
    counter = 0;
  }

  static uint32_t speedTimer = millis();
  if ((millis() - speedTimer) >= 1000) {
    speedTimer = millis();
    Serial.print("The wheel speed is ");
    Serial.print(GearedWheelSpeed(speedHz));
    Serial.print("RPM controlled by PID output of PWM at ");
    Serial.print(Output);
    Serial.print(".    The Setpoint is  ");
    Serial.print(Setpoint);
    Serial.print(". The input is ");
    Serial.println(Input);
    speedHz = 0;
  }

  double gap = abs(Setpoint - Input); //distance away from setpoint
  if (gap < 1)
  { //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
    //we're far from setpoint, use aggressive tuning parameters
    myPID.SetTunings(aggKp, aggKi, aggKd);
  }

  myPID.Compute();
  analogWrite(analogOutPin, Output);
}
