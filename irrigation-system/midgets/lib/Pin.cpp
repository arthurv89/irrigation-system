#include "Arduino.h"
#include "Pin.h"

void _digitalWrite(int pin, int level) {
  Serial.println("Turning pin " + String(pin) + " -> " + String(level));
  digitalWrite(pin, level);
  delay(100);
}
