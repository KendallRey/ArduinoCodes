#include <SPI.h>
#include <RH_NRF24.h>

// Singleton instance of the radio driver
RH_NRF24 nrf24(9, 10); // CE and CSN pins

void setup() {
    Serial.begin(9600);
    if (!nrf24.init()) {
        Serial.println("NRF24 init failed");
        while (1); // Hang if initialization failed
    } else {
        Serial.println("NRF24 init succeeded");
    }
    // Set the radio frequency channel
    nrf24.setChannel(1);
    // Set the data rate
    nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm);
    // Receiver mode
    nrf24.setModeRx();
}

void loop() {
    if (nrf24.available()) {
        uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (nrf24.recv(buf, &len)) {
            buf[len] = '\0'; // Null terminate the string
            Serial.print("Received: ");
            Serial.println((char*)buf);
        } else {
            Serial.println("Receive failed");
        }
    }
}
