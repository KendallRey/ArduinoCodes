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

    #pragma region Default Servo Values

    const int MAIN_REST_POS = 90;
    const int MAIN_MAX = 180;
    const int MAIN_MIN = 0;
    const int MAIN_POS = 90;

    const int SUB_REST_POS = 90;
    const int SUB_START_POS = 180;
    const int SUB_END_POS = 0;

    const int FOOT_REST_POS = 90;
    const int FOOT_START_POS = 180;
    const int FOOT_END_POS = 0;

    const int SPEED = 1;

    #pragma endregion

    #pragma region Holder Variables

    int delaySpeed = 10;
    bool isMoving = false;
    unsigned long previousMillis = 0;
    int increment = SPEED;

    #pragma endregion

    #pragma region Main Servo

    Servo mainServo;
    int mainServoRestPos = MAIN_REST_POS;
    int mainServoMax = MAIN_MAX;
    int mainServoMin = MAIN_MIN;
    int mainServoPos = MAIN_POS;

    #pragma endregion

    #pragma region Sub Servo

    Servo subServo;
    int subServoRestPos = SUB_REST_POS;
    int subServoPos = SUB_REST_POS;
    int subServoStartPos = SUB_START_POS;
    int subServoEndPos = SUB_END_POS;
    bool isSubServoReverseStart = false;

    #pragma endregion

    #pragma region Foot Servo

    Servo footServo;
    int footServoRestPos = FOOT_REST_POS;
    int footServoPos = FOOT_REST_POS;
    int footServoStartPos = FOOT_START_POS;
    int footServoEndPos = FOOT_END_POS;

    #pragma endregion

    #pragma region Main Servo Functions

    void attachMainServo (int pin, int restPos) {
      mainServo.attach(pin);
      mainServoRestPos = restPos;
    }

    void setMainServoMinMaxPos (int minPos, int maxPos) {
      mainServoMin = minPos;
      mainServoMax = maxPos;
    }

    #pragma endregion

    #pragma region Sub Servo Functions

    void attachSubServo (int pin, int restPos) {
      subServo.attach(pin);
      subServoRestPos = restPos;
    }

    void setSubServoStartAndEndPos (int startPos, int endPos) {
      subServoStartPos = startPos;
      subServoEndPos = endPos;
    }

    #pragma endregion

    #pragma region Foot Servo Functions

    void attachFootServo (int pin, int restPos) {
      footServo.attach(pin);
      footServoRestPos = restPos;
    }

    void setFootServoStartAndEndPos (int startPos, int endPos) {
      footServoStartPos = startPos;
      footServoEndPos = endPos;
    }

    #pragma endregion

  public:

    ServoLeg() {
      mainServoPos = 90;
      subServoStartPos = 180;
      subServoEndPos = 0;
    }

    #pragma region Init Main Servo

    void initMain(int _servoPin, int _centerValue) {
      attachMainServo(_servoPin, _centerValue);
    }
  
    void initMain(int _servoPin, int _centerValue, bool reverseStart) {
      attachMainServo(_servoPin, _centerValue);
      if(reverseStart) {
        increment = -increment;
      }
    }

    void initMain(int _servoPin, int _centerValue, bool reverseStart, int _min, int _max) {
      attachMainServo(_servoPin, _centerValue);
      setMainServoMinMaxPos(_min, _max);
    }

    #pragma endregion
  
    #pragma region Init Sub Servo

    void initSub(int _servoPin, int _centerValue) {
      attachSubServo(_servoPin, _centerValue);
    }
  
    void initSub(int _servoPin, int _centerValue, bool reverseStart) {
      attachSubServo(_servoPin, _centerValue);
      isSubServoReverseStart = reverseStart;
      if(reverseStart){
        setSubServoStartAndEndPos(0, 180);
      }
    }

    void initSub(int _servoPin, int _centerValue, bool reverseStart, int startPos, int endPos) {
      attachSubServo(_servoPin, _centerValue);
      if(reverseStart){
        setSubServoStartAndEndPos(endPos, startPos);
      }
      else {
        setSubServoStartAndEndPos(startPos, endPos);
      }
    }

    void initSub(int _servoPin, int _centerValue, bool reverseStart, int startPos, int endPos, int offset) {
      attachSubServo(_servoPin, _centerValue);
      if(reverseStart){
        setSubServoStartAndEndPos(endPos, startPos);
      }
      else {
        setSubServoStartAndEndPos(startPos, endPos);
      }
    }

    #pragma endregion

    #pragma region Init Foot Servo

    void initFoot(int _servoPin, int _centerValue) {
      attachFootServo(_servoPin, _centerValue);
    }
  
    void initFoot(int _servoPin, int _centerValue, bool reverseStart) {
      attachFootServo(_servoPin, _centerValue);
      if(reverseStart){
        setFootServoStartAndEndPos(0, 180);
      }
    }

    void initFoot(int _servoPin, int _centerValue, bool reverseStart, int startPos, int endPos) {
      attachFootServo(_servoPin, _centerValue);
      if(reverseStart){
        setFootServoStartAndEndPos(endPos, startPos);
      }
      else {
        setFootServoStartAndEndPos(startPos, endPos);
      }
    }

    void initFoot(int _servoPin, int _centerValue, bool reverseStart, int startPos, int endPos, int offset) {
      attachFootServo(_servoPin, _centerValue);
      if(reverseStart){
        setFootServoStartAndEndPos(endPos, startPos);
      }
      else {
        setFootServoStartAndEndPos(startPos, endPos);
      }
    }

    #pragma endregion

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
          subServoPos = subServoRestPos;
          isMoving = false;
        } else {
          if (!isMoving) {
            isMoving = true;
            increment = SPEED;
          }
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
            // 
            if(subServoPos == subServoStartPos) {
              subServoPos = subServoEndPos;
            }
            else {
              subServoPos = subServoStartPos;
            }
            // Reverse Main Servo Rotation
            increment = -increment;
          }

        }
      Serial.println(mainServoPos);
      mainServo.write(mainServoPos);
      subServo.write(subServoPos);
    }
};

ServoLeg legFL;
ServoLeg legFR;

unsigned long previousMillis = 0;
int delaySpeed = 10; 

void setupServo() {
  legFL.initMain(5, 90, false, 40, 140);
  // legFR.initMain(6, 90, true, 40, 140);
  legFL.initSub(6, 90, false, 30, 150);
  // legFR.initSub(6, 90, true, 30, 150);
  // legFL.initFoot(5, 90, false, 30, 150);
  // legFR.initFoot(6, 90, true, 30, 150);
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
      // legFR.move(angleX);
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
