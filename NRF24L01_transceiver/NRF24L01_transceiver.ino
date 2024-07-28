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
// #include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

const byte addresses[][6] = {"00001", "00002"};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(4, addresses[1]);
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  radio.startListening();
  if (radio.available()) {
    while (radio.available()){
      char text[32] = "";
      radio.read(&text, sizeof(text));
      Serial.println(text);

    }
  }
  delay(10);
  radio.stopListening();
  const char text[] = "Hello World";
  // const char text[] = "World Hello";
  radio.write(&text, sizeof(text));
}