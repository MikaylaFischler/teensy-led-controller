#include <Arduino.h>
#include "Ethernet.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(13, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalToggle(13);
  delay(500);
}
