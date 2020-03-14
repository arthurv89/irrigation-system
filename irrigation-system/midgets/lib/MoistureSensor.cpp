#include "Arduino.h"
#include "MoistureSensor.h"

const int moisture_sensor_in = A0;
const int voltage_pin = D2;

const int polarity = 0;

int get_moisture_value() {
  int value = read_moisture_value(moisture_sensor_in, voltage_pin);
  read_moisture_value(voltage_pin, moisture_sensor_in); // Reverse polarity and throw the result away

  return value;
}

int read_moisture_value(int in_pin, int voltage_pin) {
    pinMode(in_pin, INPUT);
    pinMode(voltage_pin, OUTPUT);

    digitalWrite(voltage_pin, HIGH);
    int value = analogRead(in_pin);
    digitalWrite(voltage_pin, LOW);

    pinMode(voltage_pin, INPUT);

    return value;
}
