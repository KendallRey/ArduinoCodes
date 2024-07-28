const int xPin = A0;
const int yPin = A1;
const int buttonPin = 7;

const int upLed = 10;
const int downLed = 6;
const int leftLed = 11;
const int rightLed = 5;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(upLed, OUTPUT);
  pinMode(downLed, OUTPUT);
  pinMode(leftLed, OUTPUT);
  pinMode(rightLed, OUTPUT);
}

void loop() {
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);
  int buttonVal = digitalRead(buttonPin);

  setLedColor(yVal, xVal);

  int mappedX = map(xVal, 0, 1023, -1024, 1024);
  int mappedY = map(yVal, 0, 1023, 1024, -1024);

  Serial.print("X = ");
  Serial.print(mappedX);

  Serial.print("     Y = ");
  Serial.print(mappedY);

  Serial.print("     Button = ");
  Serial.print(buttonVal);

  Serial.println("");
  delay(100);
}

void setLedColor(int y, int x) {
  int upPWM = map(y, 511, 0, 0,255);
  int downPWM = map(y, 511, 1023, 0,255);
  int leftPWM = map(x, 495, 0, 0, 255);
  int rightPWM = map(x, 495, 1023, 0, 255);

  if(y <= 500){
    analogWrite(upLed, upPWM);
    analogWrite(downLed, 0);
  }
  else if(y >= 550){
    analogWrite(downLed, downPWM);
    analogWrite(upLed, 0);
  }
  else {
    analogWrite(upLed, 0);
    analogWrite(downLed, 0);
  }
  if(x <= 450){
    analogWrite(leftLed, leftPWM);
    analogWrite(rightLed, 0);
  }
  else if(x >= 550){
    analogWrite(rightLed, rightPWM);
    analogWrite(leftLed, 0);
  }
  else {
    analogWrite(leftLed, 0);
    analogWrite(rightLed, 0);
  }
}
