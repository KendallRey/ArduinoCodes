
// UNO PINOUT
// 5v -> VCC, MAKE SURE YOU HAVE ADAPTER!!!
// GND -> GND
// 8 -> CE
// 10 -> CSN
// 11 -> MOSI
// 12 -> MISO
// 13 -> SCK
// UNO PINOUT

// UNO NANO PINOUT
// 5v -> VCC, MAKE SURE YOU HAVE ADAPTER!!!
// GND -> GND
// 8 -> CE
// 9 -> CSN
// 11 -> MOSI
// 12 -> MISO
// 13 -> SCK
// UNO NANO PINOUT

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

const int xPin = A0;
const int yPin = A1;

//create an RF24 object
RF24 radio(8, 10);   // CE, CSN

const byte address[6] = "11229";

int intReadings[3] = {0,0,0};

void setup() {
  Serial.begin(9600);
  setupRadio();
}

void loop() {
  readAnalog();
  sendData();
  for (int i = 0; i < 3; i++) {
    Serial.print(intReadings[i]);
    if (i < 2) Serial.print(", ");
  }
  Serial.println();
  delay(10);
}

void setupRadio() {
  bool isBegin = radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  if(isBegin)
    Serial.println("Start Sending Init");
  else
    Serial.println("Failed Sending Init");
  delay(500);
}

void readAnalog(){
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);

  intReadings[0] = map(xVal, 0, 1023, -1024, 1024);
  intReadings[1] = map(yVal, 0, 1023, 1024, -1024);
}

void sendData(){
  bool success = radio.write(&intReadings, sizeof(intReadings));
}