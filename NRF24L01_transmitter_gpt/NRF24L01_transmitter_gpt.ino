#include <SPI.h>
#include <RF24.h>

// Define the pins for CE and CSN
#define CE_PIN 8
#define CSN_PIN 7

// Create an RF24 object
RF24 radio(CE_PIN, CSN_PIN);

// Define the unique address
const uint64_t address = 0xDEADBEEFC0LL;

void setup() {
  // Initialize the serial monitor
  Serial.begin(9600);
  
  // delay(3000);
  Serial.println("Transmitter Initialize ~");
  // Initialize the radio
  // radio.begin();
  bool isBegin = radio.begin();
  if(isBegin)
    Serial.println("Transmitter Started!");
  else 
    Serial.println("Transmitter FAILED to Start!");
  
  radio.openWritingPipe(address);
  // radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
  radio.setChannel(110);//set frequency to channel 110
  Serial.println("");
  Serial.println("Start Sending ~");
  delay(3000);
}

void loop() {
  // Send a message
  const char text[] = "Hello, world!";
  bool isSent = radio.write(&text, sizeof(text));

  // Print the sent message to the serial monitor
  if(isSent)
    Serial.println("Sent: Hello, world!");
  else
    Serial.println("Sending Failed!");

  // Wait for a while before sending the next message
  delay(500);
}
