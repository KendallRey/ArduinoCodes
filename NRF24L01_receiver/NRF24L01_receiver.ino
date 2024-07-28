/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

// UNO
// 3.3v -> VCC
// GND -> GND
// 9 -> CE
// 8 -> CSN
// 11 -> MOSI
// 12 -> MISO
// 13 -> SCK

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(8, 10);  // CE, CSN

const byte address[6] = "11009";

void setup() {
  Serial.begin(9600);
  bool isBegin = radio.begin();
  radio.openReadingPipe(0, address);
  // radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  if(isBegin)
    Serial.println("Start Listening Init");
  else
    Serial.println("Failed Listening Init");
  delay(500);
}

void loop() {
  if (radio.available()) {
    char text[32] = "334";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}