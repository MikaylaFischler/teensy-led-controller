#include <Arduino.h>

#include "Ethernet.h"

uint8_t MAC[6] = { 0x00, 0x4D, 0x49, 0x4B, 0x00, 0x01 };

void setup() {
    pinMode(13, HIGH);
    digitalWrite(13, LOW);

    delay(5000);
    digitalWrite(13, HIGH);

    Serial.begin(115200);

    // setup ethernet link and request IP over DHCP
    Ethernet::init(MAC);
    Ethernet::req_ip();
}

void loop() {
    digitalToggle(13);
    delay(500);
}
