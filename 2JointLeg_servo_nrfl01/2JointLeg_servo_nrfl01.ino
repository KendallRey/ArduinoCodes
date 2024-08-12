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
    Servo mainServo;
    Servo subServo;

    int mainRestPos = 90;
    int subRestPos = 90;

    bool isSubReverseStart = false;
  
    int subStartPos = 180;
    int subEndPos = 0;
  
    int delaySpeed = 10;
  
    int mainPos = 0;
    int subPos = 0;
    

    unsigned long previousMillis = 0;
    int increment = 1;

    int max = 180;
    int min = 0;

  public:

    ServoLeg() {
      mainPos = 90;
      subStartPos = 180;
      subEndPos = 0;
    }

    void initMain(int _servoPin, int _centerValue) {
      mainServo.attach(_servoPin);
      mainRestPos = _centerValue;
      mainPos = _centerValue;
    }
  
    void initMain(int _servoPin, int _centerValue, bool reverseStart) {
      mainServo.attach(_servoPin);
      mainRestPos = _centerValue;
      mainPos = _centerValue;
      if(reverseStart) {
        increment = -increment;
      }
    }

    void initMain(int _servoPin, int _centerValue, bool reverseStart, int _min, int _max) {
      mainServo.attach(_servoPin);
      mainRestPos = _centerValue;
      mainPos = _centerValue;
      max = _max;
      min = _min;
    }
  
    void initSub(int _servoPin, int _centerValue) {
      subServo.attach(_servoPin);
      subRestPos = _centerValue;
      mainPos = _centerValue;
    }
  
    void initSub(int _servoPin, int _centerValue, bool reverseStart) {
      subServo.attach(_servoPin);
      subRestPos = _centerValue;
      mainPos = _centerValue;
      isSubReverseStart = reverseStart;
      if(reverseStart){
        subStartPos = 0;
        subEndPos = 180;
      }
    }

    void initSub(int _servoPin, int _centerValue, bool reverseStart, int offset) {
      subServo.attach(_servoPin);
      subRestPos = _centerValue;
      mainPos = _centerValue;
      if(reverseStart){
        subStartPos = 0;
        subEndPos = 180;
      }
    }

    void moveDelayed(int speed) {

      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= delaySpeed) {
        previousMillis = currentMillis;
        if(speed <= 2 && speed >= -2){
          mainPos = mainRestPos;
        } else {

          if(speed > 1) {
            mainPos += increment;
          }
          else if (speed < -1) {
            mainPos -= increment;
          }
          if (mainPos >= max || mainPos <= min) {
            increment = -increment;
          }
        }
        mainServo.write(mainPos);
      }
    }

    void move(int speed) {
      if(speed <= 2 && speed >= -2){
          mainPos = mainRestPos;
          subPos = subRestPos;
        } else {
          
          if(speed > 1) {
            mainPos += increment;
          }
          else if (speed < -1) {
            mainPos -= increment;
          }
          if (isSubReverseStart){
            subPos = 180;
          }
          if (mainPos >= max || mainPos <= min) {
            if(subPos == subStartPos)
              subPos = subEndPos;
            else
              subPos = subStartPos;
            increment = -increment;
          }

        }
      mainServo.write(mainPos);
      subServo.write(subPos);
    }
};

ServoLeg legFL;
ServoLeg legFR;

unsigned long previousMillis = 0;
int delaySpeed = 10; 

void setupServo() {
  // legFL.initMain(5, 90, false, 30, 150);
  // legFR.initMain(6, 90, true, 30, 150);
  legFL.initSub(5, 90, false);
  legFR.initSub(6, 90, true);
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
