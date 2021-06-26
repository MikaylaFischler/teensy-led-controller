#include <Arduino.h>

#include "Ethernet.h"

uint8_t MAC[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

void setup() {
    pinMode(13, HIGH);

    delay(5000);

    Serial.begin(115200);

    Ethernet::init(MAC);
    Ethernet::req_ip();
}

void loop() {
    digitalToggle(13);
    delay(500);
}
