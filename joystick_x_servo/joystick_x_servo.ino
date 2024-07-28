#include <Servo.h>

const int xPin = A0;
const int yPin = A1;

const int servoXPin = 6;
const int servoYPin = 5;

Servo servoX;
Servo servoY;

void setup() {
  // put your setup code here, to run once:
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);
}

void loop() {
  // put your main code here, to run repeatedly:

  int readingX = analogRead(xPin);
  int angleX = map(readingX, 0, 1023, 0, 180);

  int readingY = analogRead(yPin);
  int angleY = map(readingY, 0, 1023, 0, 180);

  servoX.write(angleX);
  servoY.write(angleY);

}
