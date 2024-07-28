// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dEpendant SPI Library
#include <SPI.h>
// Create Amplitude Shift Keying Obj ect

// Use PIN 11 for data

RH_ASK rf_driver;

void setup() {
  // put your setup code here, to run once:
  rf_driver.init();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buf[24];
  uint8_t buflen = sizeof(buf);

  if(rf_driver.recv(buf, &buflen)){
    Serial.print("Message Received: ");
    Serial.println((char*)buf);
  }
}
