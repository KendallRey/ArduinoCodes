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
  Serial.println("Receiver Initialize ~");
  // Initialize the radio
  bool isBegin = radio.begin();
  if(isBegin)
    Serial.println("Receiver Started!");
  else 
    Serial.println("Receiver FAILED to Start!");
  
  radio.openReadingPipe(0, address);
  // radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  radio.setChannel(110);//set frequency to channel 110
  Serial.println("");
  Serial.println("Start Listening ~");
  delay(3000);
}

void loop() {
  // Check if there is data available to read
  if (radio.available()) {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    
    // Print the received message to the serial monitor
    Serial.print("Received: ");
    Serial.println(text);
  }
}
