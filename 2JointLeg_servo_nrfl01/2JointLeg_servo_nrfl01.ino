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
