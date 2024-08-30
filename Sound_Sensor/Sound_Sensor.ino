
int dPin = 7;
int aPin = A0;

int RelayPin = 6;

int digitalVal;
int analogVal;

bool isOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(dPin, INPUT);
  pinMode(aPin, INPUT);
	pinMode(RelayPin, OUTPUT);

  
  digitalWrite(RelayPin, HIGH);
}

void loop() {
  digitalVal = digitalRead(dPin);
  analogVal = analogRead(aPin);
  Serial.print("A: ");
  Serial.print(analogVal);
  bool current = isOn;
  // if(digitalVal == HIGH){
  if(analogVal <= 115){
    Serial.print("HIGH: ");
  }
  else {
    isOn = !isOn;
    if(current != isOn){
      delay(500);
    }
    if(isOn){
      digitalWrite(RelayPin, LOW);
    }
    else {
      digitalWrite(RelayPin, HIGH);
    }
    Serial.print("LOW: ");
  }
  Serial.print(isOn);
  Serial.println("");
  delay(10);
}
