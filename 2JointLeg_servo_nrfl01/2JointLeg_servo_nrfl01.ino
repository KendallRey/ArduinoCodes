
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
    int center;
    int delaySpeed = 1;
  
    int pos = 0;
    unsigned long previousMillis = 0;
    int increment = 1;

  public:

    // Constructor
    ServoLeg(int _servoPin, int _centerValue, int _delaySpeed) {
      mainServo.attach(_servoPin);
      center = _centerValue;
      delaySpeed = _delaySpeed;
    }

    void move(int speed) {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= delaySpeed) {
        // Save the last time you updated the servo position
        previousMillis = currentMillis;
          if(speed <= 1 && speed >= -1){
        pos = center;
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
        mainServo.write(pos);
      }
    }

    void move2(int speed) {
      if(speed <= 1 && speed >= -1){
        pos = center;
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
      mainServo.write(pos);
    }
};

// ServoLeg legFL(6, 90, 1);


int increment = 1;
int center = 90;
int pos = 0;
unsigned long previousMillis = 0;
int delaySpeed = 10; 


//create servo object
Servo servoX;
Servo servoY;
const int servoXPin = 5;
const int servoYPin = 6;

void setupServo() {
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);
}

void setup() {
  Serial.begin(9600);
  setupRadio();
  setupServo();
}

void loop() {
  readData();


  // Serial.println(potValue);
  int angleX = map(intReadings[1], -1023, 1023, -20, 20);
  // int val = map(potValue, -1023, 1023, -20, 20);
  
  // Serial.println(angleX);
  // servoX.write(angleX);
  moveServo(angleX);
  // delay(delaySpeed);
}

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
