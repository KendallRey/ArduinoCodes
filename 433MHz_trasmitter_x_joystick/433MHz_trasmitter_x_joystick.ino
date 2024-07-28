// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dEpendant SPI Library
#include <SPI.h>
// Create Amplitude Shift Keying Obj ect

const int xPin = A0;
const int yPin = A1;
const int buttonPin = 7;

RH_ASK rf_driver;

void setup() {
  // put your setup code here, to run once:
  rf_driver.init();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);
  
  int mappedX = map(xVal, 0, 1023, -1024, 1024);
  int mappedY = map(yVal, 0, 1023, 1024, -1024);
  String paddedX = padNumberToString(mappedX, 6);
  String paddedY = padNumberToString(mappedY, 6);

  const String concat = paddedX+","+paddedY;
  const char *msg = concat.c_str();

  // Serial.print("X = ");
  // Serial.print(paddedX);

  // Serial.print("     Y = ");
  // Serial.print(paddedY);
  // Serial.println(" ");

  rf_driver.send((uint8_t *)msg,strlen(msg));
  rf_driver.waitPacketSent() ;
  // delay(15);

}


String padNumberToString(int value, int padding) {
  char buffer[padding+1];
  String str = String(value);

  // Calculate padding length
  int paddingLength = padding - str.length();

  // Create padded string
  if (paddingLength > 0) {
    for (int i = 0; i < paddingLength; i++) {
      buffer[i] = '0';
    }
    str.toCharArray(buffer + paddingLength, str.length() + 1);
  } else {
    str.toCharArray(buffer, str.length() + 1);
  }

  return buffer;
}
