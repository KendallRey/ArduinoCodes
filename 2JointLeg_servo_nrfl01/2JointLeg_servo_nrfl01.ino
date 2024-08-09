
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>


int potValue;  // value from the analog pin
int escValue;  // value to send to the ESC

//create an RF24 object
RF24 radio(8, 9);  // CE, CSN

const byte address[6] = "11229";

int intReadings[3] = {0,0,0};



class ServoLeg {
  private:
    Servo mainServo; 
    int center = 90;
    int delaySpeed = 10;
  
    int pos = 0;
    unsigned long previousMillis = 0;
    int increment = 1;

    int max = 180;
    int min = 0;

  public:

    ServoLeg() {
      pos = 90;
    }

    void init(int _servoPin, int _centerValue) {
      mainServo.attach(_servoPin);
      center = _centerValue;
      pos = _centerValue;
    }
  
    void init(int _servoPin, int _centerValue, bool reverseStart) {
      mainServo.attach(_servoPin);
      center = _centerValue;
      pos = _centerValue;
      if(reverseStart) {
        increment = -increment;
      }
    }

    void init(int _servoPin, int _centerValue, bool reverseStart, int _min, int _max) {
      mainServo.attach(_servoPin);
      center = _centerValue;
      pos = _centerValue;
      max = _max;
      min = _min;
    }

    void moveDelayed(int speed) {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= delaySpeed) {
        // Save the last time you updated the servo position
        previousMillis = currentMillis;
        if(speed <= 2 && speed >= -2){
          pos = center;
        } else {

          if(speed > 1) {
            pos += increment;
          }
          else if (speed < -1) {
            pos -= increment;
          }
          
          if (pos >= max || pos <= min) {
            increment = -increment;
          }
        }
        mainServo.write(pos);
      }
    }

    void move(int speed) {
      if(speed <= 2 && speed >= -2){
          pos = center;
        } else {

          if(speed > 1) {
            pos += increment;
          }
          else if (speed < -1) {
            pos -= increment;
          }
          
          if (pos >= max || pos <= min) {
            increment = -increment;
          }
        }
      mainServo.write(pos);
    }
};

ServoLeg legFL;
ServoLeg legFR;

unsigned long previousMillis = 0;
int delaySpeed = 10; 

void setupServo() {

  legFL.init(5, 90, false, 30, 150);
  legFR.init(6, 90, true, 30, 150);
}

void setup() {
  Serial.begin(9600);
  setupRadio();
  setupServo();
  delay(500);
}

void loop() {
  readData();

  // Serial.println(potValue);
  // int val = map(potValue, -1023, 1023, -20, 20);
  moveLegs();
  // servoX.write(angleX);
  // moveServo(angleX);
  // delay(delaySpeed);
}


void moveLegs() {
  
  int angleX = map(intReadings[1], -1023, 1023, -20, 20);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= delaySpeed) {
      // Save the last time you updated the servo position
      previousMillis = currentMillis;
      legFL.move(angleX);
      legFR.move(angleX);
    }
};

void setupRadio() {
  bool isBegin = radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  if(isBegin)
    Serial.println("Start Listening Init");
  else
    Serial.println("Failed Listening Init");
  delay(500);
}

void readData() {
  if (radio.available()) {
    int data[3] = {0};
    radio.read(&data, sizeof(data));
    for (int i = 0; i < 3; i++) {
      intReadings[i] = data[i];
    }
  }
}

void moveServo(int speed) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= delaySpeed) {
    // Save the last time you updated the servo position
    previousMillis = currentMillis;
    if(speed <= 2 && speed >= -2){
      pos = 90;
    } else {

      if(speed > 1) {
        pos += increment;
      }
      else if (speed < -1) {
        pos -= increment;
      }
      
      if (pos >= 180 || pos <= 0) {
        increment = -increment;
      }
    }
      Serial.println(pos);
      servoX.write(pos);
  }
}


void moveServo2(int speed) {
  if(speed <= 2 && speed >= -2){
      pos = 90;
    } else {

    if(speed > 1) {
      pos += increment;
    }
    else if (speed < -1) {
      pos -= increment;
    }
    
    if (pos >= 180 || pos <= 0) {
      increment = -increment;
    }
    Serial.println(pos);
    servoX.write(pos);
  }
}
