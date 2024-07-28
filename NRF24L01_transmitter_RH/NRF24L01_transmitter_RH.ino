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
}

void loop() {
    const char* msg = "Hello, World!";
    nrf24.send((uint8_t *)msg, strlen(msg));
    nrf24.waitPacketSent();
    Serial.println("Sent: Hello, World!");
    delay(1000);
}
