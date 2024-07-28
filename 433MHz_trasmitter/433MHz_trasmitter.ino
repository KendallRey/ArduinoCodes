// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dEpendant SPI Library
#include <SPI.h>
// Create Amplitude Shift Keying Obj ect

// Use PIN 12 for data

RH_ASK rf_driver;

void setup() {
  // put your setup code here, to run once:
  rf_driver.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  const char *msg = "Welcome to the Workshop!";
  rf_driver.send((uint8_t *)msg,strlen(msg));
  rf_driver.waitPacketSent() ;
  delay(1000);
}
