
//Include the Arduino stepper library
#include <Stepper.h>

// Number of steps per internal motor revolution
const float STEPS_PER_REV = 32;

// Amount of Gear Reduction
const float GEAR_RED = 64;

// Number of steps per geared output rotation
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;

// Define Variables
int stepsRequired;

//Use pin 8-11 to IN1-IN4
Stepper stepper = Stepper(STEPS_PER_REV, 8, 10, 9, 11);

void setup() {

}

void loop() {

  stepper.setSpeed(1);
  stepsRequired = 4;
  stepper.step(stepsRequired);
  delay(2000);

  stepsRequired = STEPS_PER_OUT_REV / 2;
  stepper.setSpeed(100);
  stepper.step(stepsRequired);
  delay(1000);

  stepsRequired = - STEPS_PER_OUT_REV / 2;
  stepper.setSpeed(700);
  stepper.step(stepsRequired);
  delay(2000);

}

void setStepperSpeed(long _speed,  float _stepsRequired, long _delay) {
  stepper.setSpeed(_speed);
  stepsRequired = _stepsRequired;
  stepper.step(stepsRequired);
  delay(_delay);
}