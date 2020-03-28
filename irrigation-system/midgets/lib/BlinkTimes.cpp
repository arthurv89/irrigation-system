#include "BlinkTimes.h"
#include "Arduino.h"
#include "Pin.h"

void blink_reverse(int pin, int times) {
  do_blink(pin, times, LOW, HIGH);
}

void blink_default(int pin, int times) {
  do_blink(pin, times, HIGH, LOW);
}

void do_blink(int pin, int times, int on_value, int off_value) {
  pinMode(pin, OUTPUT);
  for(int i=0; i<times; i++) {
    _digitalWrite(pin, on_value);
    Serial.println("On");
    delay(200);

    _digitalWrite(pin, off_value);
    Serial.println("Off");
    delay(200);
  }
}
