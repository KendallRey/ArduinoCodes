#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver();

#define servoMIN 150
#define servoMAX 600

void setup() {
  Serial.begin(9600);
  pwmDriver.begin();
  pwmDriver.setPWMFreq(60);

}

void loop() {
  for (int servo = 0; servo < 16; servo++) {
    pwmDriver.setPWM(servo, 0, servoMIN);
    Serial.println(servo);
    delay(500);
  }
  for (int servo = 3; servo >= 0; servo--) {
      pwmDriver.setPWM(servo, 0, servoMAX);
      Serial.println(servo);
      delay(500);
  }
}
