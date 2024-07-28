/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
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
RF24 radio(8, 10);   // CE, CSN

const byte address[6] = "11009";

void setup() {
  Serial.begin(9600);
  bool isBegin = radio.begin();
  radio.openWritingPipe(address);
  // radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  if(isBegin)
    Serial.println("Start Sending Init");
  else
    Serial.println("Failed Sending Init");
  delay(500);
}

void loop() {
  const char text[] = "Hello World";
  
  bool isSent = radio.write(&text, sizeof(text));

  Serial.print("Send:" );
  Serial.println(isSent);
  delay(10);
}