#include <AccelStepper.h>

#define FULLSTEP 4
#define HALFSTEP 8

#define motorPin1 8
#define motorPin2 9
#define motorPin3 10
#define motorPin4 11

// The sequence 1-3-2-4 is required
AccelStepper stepper = AccelStepper(FULLSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  // put your setup code here, to run once:
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(50);
  stepper.setSpeed(200);
  stepper.moveTo(2048);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(stepper.distanceToGo() == 0){
    stepper.moveTo(-stepper.currentPosition());
  }

  stepper.run();
}
