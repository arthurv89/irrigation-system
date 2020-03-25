#include "Arduino.h"
#include "MoistureSensor.h"

const int polarity = 0;

int get_moisture_value(int moisture_sensor_in, int pin1, int pin2) {
  pinMode(moisture_sensor_in, INPUT);

  int value = read_moisture_value(moisture_sensor_in, pin1, pin2);
  read_moisture_value(moisture_sensor_in, pin2, pin1); // Reverse polarity and throw the result away

  return value;
}

int read_moisture_value(int moisture_sensor_in, int ground_pin, int voltage_pin) {
    digitalWrite(ground_pin, LOW);
    pinMode(ground_pin, INPUT);

    pinMode(voltage_pin, OUTPUT);
    digitalWrite(voltage_pin, HIGH);

    int value = analogRead(moisture_sensor_in);

    digitalWrite(ground_pin, LOW);
    pinMode(ground_pin, INPUT);

    digitalWrite(voltage_pin, LOW);
    pinMode(voltage_pin, INPUT);

    return value;
}
