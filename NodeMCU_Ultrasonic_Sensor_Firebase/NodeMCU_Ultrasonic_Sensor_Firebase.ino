#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Replace these with your WiFi credentials
#define WIFI_SSID "WIFI-SSID"
#define WIFI_PASSWORD "WIFI-PASSWORD"

// Replace with your Firebase project credentials
#define API_KEY "key-Pu8zvccM"
#define DATABASE_URL "url"  // Example: your-project-id.firebaseio.com
#define USER_EMAIL "email"
#define USER_PASSWORD "password"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

FirebaseData  firebaseData;

#define TRIG_PIN 12
#define ECHO_PIN 14

void setup() {
  Serial.begin(115200);
  initWiFi();
  setupFirebase();
  setupUltrasonicSensor();

}
int openCounter = 1;
bool isOpen = false;
bool isOpened = false;

void loop() {
  // Send a 10 microsecond pulse to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo time
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate the distance in cm
  float distance = (duration * 0.0343) / 2;

  if (Firebase.isTokenExpired()){
    Firebase.refreshToken(&config);
    Serial.println("Refresh token");
  } else {
    if(distance <= 12) {
      isOpen = true;
      writeToDatabase(true);
      isOpened = true;
    }
    else if(distance >= 20 && isOpened) {
      isOpen = false;
      isOpened = false;
      writeToDatabase(false);
    }
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(500);

  if (isOpen)
    delay(5000);
}

void setupUltrasonicSensor() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setupFirebase() {

  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  delay(2000);

}

void writeToDatabase(bool isOpen) {
  String path = "/door/status/default";
  if(isOpen) {
    path = "/door/status/open-" + String(openCounter) + "-A";
  }
  else {
    path = "/door/status/open-" + String(openCounter) + "-B";
  }


  if (Firebase.RTDB.setTimestamp(&fbdo, path)) {
    Serial.println("Door Status Logged!");
  } else {
    Serial.println("Failed to log door status.");
  }
  openCounter++;
}
