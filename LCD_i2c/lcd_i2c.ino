
// |———————————————————————————————————————————————————————| 
// |  made by Arduino_uno_guy 11/13/2019                   |
// |   https://create.arduino.cc/projecthub/arduino_uno_guy|
//  |———————————————————————————————————————————————————————|


#include "LiquidCrystal_I2C.h"
#include <Keypad.h>
#include  "Wire.h"

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal_I2C lcd(0x27,  16, 2);

const int ledPin = 12; 
int ledState = LOW;
int *ledStateP;
unsigned long prevMillis = 0;
unsigned long *prevMillisP;
const long interval = 80;

String value = "";
String intro = "KR Mozo";

void setup() {
  Serial.begin(9600);

  prevMillisP = &prevMillis;
  ledStateP = &ledState;
  pinMode(ledPin, OUTPUT);

  Serial.println("LCD Character Backpack I2C Test.");
  //initialize lcd screen
  lcd.init();
  // turn on the backlight
  lcd.backlight();
  delayedPrint(intro, 400);
  lcd.setCursor(0, 1);
  delay(2000);
}
void loop() {
  char customKey = customKeypad.getKey();
  // blinkLED();
  intervalHighLow(ledPin, ledStateP, prevMillisP, interval);
  if (customKey){
    int len = value.length();
    int index = len-1;
    index = constrain(index, 0, 15);
    if(customKey == 'D'){
      lcd.setCursor(index, 1);
      lcd.print(" ");
      lcd.setCursor(index, 1);
      value = value.substring(0, index);
    }
    else if(len <= 15){
      value = value+customKey;
      lcd.print(customKey);
    }
    Serial.println(value);
  }
}

void delayedPrint(String value, int ms) {
  
  for (int c=0; c<value.length(); c++){
    lcd.print(value[c]);
    delay(ms);
  }

}

void intervalHighLow(int pin, int* state, long* prevMillis, long interval) {

  unsigned long currentMillis = millis();

  if (currentMillis - *prevMillis >= interval) {
    *prevMillis = currentMillis;
    Serial.println(*prevMillis);
    if (*state == LOW) {
      *state = HIGH;
    } else {
      *state = LOW;
    }

    digitalWrite(pin, *state);
  }
}

