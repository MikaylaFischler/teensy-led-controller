#include <Arduino.h>

#include "Ethernet.h"
#include "Logger.h"

#define TCL_VERSION "0.0.1"

uint8_t MAC[6] = { 0x00, 0x4D, 0x49, 0x4B, 0x00, 0x01 };

void setup() {
    pinMode(13, HIGH);
    digitalWrite(13, LOW);

    // Initialize Serial Logging //
    Logger::init(&Serial);
    tlc_println("[TLC] delaying startup...");
    delay(5000);

    digitalWrite(13, HIGH);
    tlc_println("=== Teensy LED Controller v" TCL_VERSION " ===");

    // Initialize Networking //
    Ethernet::init(MAC);
    Ethernet::req_ip();

    tlc_println("[TLC] startup completed");
}

void loop() {
    digitalToggle(13);
    delay(500);
}
