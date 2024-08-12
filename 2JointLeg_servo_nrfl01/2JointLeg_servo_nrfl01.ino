// Needed Libraries
// - Servo by Michael Margolis, Arduino
// - RF24 by TMRh20

// NRF24L01 -> UNO PINOUT
// 5v -> VCC, MAKE SURE YOU HAVE ADAPTER!!!
// GND -> GND
// 8 -> CE
// 10 -> CSN
// 11 -> MOSI
// 12 -> MISO
// 13 -> SCK
// UNO PINOUT

// NRF24L01 -> UNO NANO PINOUT
// 5v -> VCC, MAKE SURE YOU HAVE ADAPTER!!!
// GND -> GND
// 8 -> CE
// 9 -> CSN
// 11 -> MOSI
// 12 -> MISO
// 13 -> SCK
// UNO NANO PINOUT

// ANALOG PINOUT
// GND -> GND
// VCC -> 5V
// X PIN -> A0 (Your choice!)
// Y PIN -> A1 (Your choice!)

// SERVO PINOUT
// GND -> GND
// VCC -> 5V
// X PIN -> D5 PWM (Your choice!)
// Y PIN -> D6 PWM (Your choice!)

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(8, 9);  // CE, CSN

const byte address[6] = "11229";

int intReadings[3] = {0,0,0};

// Test Direct with Analog Connection
void testRead () {
  int analogX = analogRead(A1);
  intReadings[1] = map(analogX, 0, 1023, -1023, 1023);
}

class ServoLeg {
  private:

    #pragma region Main Servo

    Servo mainServo;
    int mainServoRestPos = 90;
    int mainServoMax = 180;
    int mainServoMin = 0;
    int mainServoPos = 0;

    #pragma endregion

    #pragma region Sub Servo

    Servo subServo;
    int subservoRestPos = 90;
    int subServoPos = 0;
    int subServoStartPos = 180;
    int subServoEndPos = 0;
    bool isSubServoReverseStart = false;

    #pragma endregion

    #pragma region Foot Servo

    Servo footServo;
    int footServoRestPos = 90;
    int footServoPos = 0;
    int footServoStartPos = 180;
    int footServoEndPos = 0;

    #pragma endregion
  
  
    int delaySpeed = 10;

    unsigned long previousMillis = 0;
    int increment = 1;


  public:

    ServoLeg() {
      mainServoPos = 90;
      subServoStartPos = 180;
      subServoEndPos = 0;
    }

    void initMain(int _servoPin, int _centerValue) {
      mainServo.attach(_servoPin);
      mainServoRestPos = _centerValue;
      mainServoPos = _centerValue;
    }
  
    void initMain(int _servoPin, int _centerValue, bool reverseStart) {
      mainServo.attach(_servoPin);
      mainServoRestPos = _centerValue;
      mainServoPos = _centerValue;
      if(reverseStart) {
        increment = -increment;
      }
    }

    void initMain(int _servoPin, int _centerValue, bool reverseStart, int _min, int _max) {
      mainServo.attach(_servoPin);
      mainServoRestPos = _centerValue;
      mainServoPos = _centerValue;
      mainServoMax = _max;
      mainServoMin = _min;
    }
  
    void initSub(int _servoPin, int _centerValue) {
      subServo.attach(_servoPin);
      subservoRestPos = _centerValue;
      mainServoPos = _centerValue;
    }
  
    void initSub(int _servoPin, int _centerValue, bool reverseStart) {
      subServo.attach(_servoPin);
      subservoRestPos = _centerValue;
      mainServoPos = _centerValue;
      isSubServoReverseStart = reverseStart;
      if(reverseStart){
        subServoStartPos = 0;
        subServoEndPos = 180;
      }
    }

    void initSub(int _servoPin, int _centerValue, bool reverseStart, int startPos, int endPos) {
      subServo.attach(_servoPin);
      subservoRestPos = _centerValue;
      mainServoPos = _centerValue;
      if(reverseStart){
        subServoStartPos = endPos;
        subServoEndPos = startPos;
      }
      else {
        subServoStartPos = startPos;
        subServoEndPos = endPos;
      }
    }

    void initSub(int _servoPin, int _centerValue, bool reverseStart, int startPos, int endPos, int offset) {
      subServo.attach(_servoPin);
      subservoRestPos = _centerValue;
      mainServoPos = _centerValue;
      if(reverseStart){
        subServoStartPos = endPos;
        subServoEndPos = startPos;
      }
      else {
        subServoStartPos = startPos;
        subServoEndPos = endPos;
      }
    }

    void moveDelayed(int speed) {

      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= delaySpeed) {
        previousMillis = currentMillis;
        if(speed <= 2 && speed >= -2){
          mainServoPos = mainServoRestPos;
        } else {

          if(speed > 1) {
            mainServoPos += increment;
          }
          else if (speed < -1) {
            mainServoPos -= increment;
          }
          if (mainServoPos >= mainServoMax || mainServoPos <= mainServoMin) {
            increment = -increment;
          }
        }
        mainServo.write(mainServoPos);
      }
    }

    void move(int speed) {
      if(speed <= 2 && speed >= -2){
          mainServoPos = mainServoRestPos;
          subServoPos = subservoRestPos;
        } else {
          
          if(speed > 1) {
            mainServoPos += increment;
          }
          else if (speed < -1) {
            mainServoPos -= increment;
          }
          if (isSubServoReverseStart){
            subServoPos = 180;
          }
          if (mainServoPos >= mainServoMax || mainServoPos <= mainServoMin) {
            if(subServoPos == subServoStartPos)
              subServoPos = subServoEndPos;
            else
              subServoPos = subServoStartPos;
            increment = -increment;
          }

        }
      mainServo.write(mainServoPos);
      subServo.write(subServoPos);
    }
};

ServoLeg legFL;
ServoLeg legFR;

unsigned long previousMillis = 0;
int delaySpeed = 10; 

void setupServo() {
  // legFL.initMain(5, 90, false, 30, 150);
  // legFR.initMain(6, 90, true, 30, 150);
  legFL.initSub(5, 90, false, 30, 150);
  legFR.initSub(6, 90, true, 30, 150);
}

void setup() {
  Serial.begin(9600);
  setupRadio();
  setupServo();
  delay(500);
}

void loop() {
  // readData();
  testRead();
  moveLegs();
}


void moveLegs() {
  
  int angleX = map(intReadings[1], -1023, 1023, -20, 20);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= delaySpeed) {
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
