#include "BlinkTimes.h"
#include "Arduino.h"

void blink_reverse(int pin, int times) {
  do_blink(pin, times, LOW, HIGH);
}

void blink_default(int pin, int times) {
  do_blink(pin, times, HIGH, LOW);
}

void do_blink(int pin, int times, int on_value, int off_value) {
  pinMode(pin, OUTPUT);
  for(int i=0; i<times; i++) {
    digitalWrite(pin, on_value);
    Serial.println("On");
    delay(200);

    digitalWrite(pin, off_value);
    Serial.println("Off");
    delay(200);
  }
}
