#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("WiFi Scan Example");

  // Initialize the WiFi module
  WiFi.mode(WIFI_STA);  // Set the ESP8266 to Wi-Fi station mode
  WiFi.disconnect();    // Disconnect from any network to start scanning cleanly
  delay(100);
}

void loop() {
  Serial.println("Scanning for Wi-Fi networks...");

  // Wi-Fi network scan
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete");
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found:");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));    // SSID (network name)
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));    // Signal strength (RSSI)
      Serial.print(" dBm)");
      Serial.print(" - Encryption: ");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "Open" : "Encrypted");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}
