
// UNO
// 5v -> VCC, MAKE SURE YOU HAVE ADAPTER!!!
// GND -> GND
// 8 -> CE
// 10 -> CSN
// 11 -> MOSI
// 12 -> MISO
// 13 -> SCK

// ANALOG
// GND -> GND
// VCC -> 5V
// X PIN -> A0 (Your choice!)
// Y PIN -> A1 (Your choice!)

// SERVO
// GND -> GND
// VCC -> 5V
// X PIN -> D5 PWM (Your choice!)
// Y PIN -> D6 PWM (Your choice!)

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//create servo object
Servo servoX;
Servo servoY;
const int servoXPin = 5;
const int servoYPin = 6;

//create an RF24 object
RF24 radio(8, 10);  // CE, CSN

const byte address[6] = "11009";

int intReadings[3] = {0,0,0};

void setup() {
  Serial.begin(9600);
  setupRadio();
  setupServo();
}

void loop() {
  readData();
  writeServoes();
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
      Serial.print(intReadings[i]);
      if (i < 2) Serial.print(", ");
    }
    Serial.println();
  }
}

void setupServo() {
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);
}

int test = 0;

void writeServoes() {
    int angleX = map(intReadings[0], -1023, 1023, 0, 180);
    int angleY = map(intReadings[1], -1023, 1023, 0, 180);

    servoX.write(180);
    servoY.write(180);
}