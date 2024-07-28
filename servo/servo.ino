#include <Servo.h>

Servo servo;

const int servoPin = 9;
const int potenPin = A0;

void setup() {
  // put your setup code here, to run once:
  servo.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(potenPin);
  int angle = map(reading, 0, 1023, 0, 180);
  servo.write(angle);
}
