#include "Arduino.h"
#include "TemperatureSensor.h"
#include"DHTLib.h"

// Import required libraries
#include <Adafruit_Sensor.h>
#include <DHT.h>

/**
  * Usage:

  * - Pins on the temp chip, with blue component aimming towards top and to the back:
  *   [D1, 3V, GND]
  *
  * - Put the following in the setup method:
  *   dht.begin();
  *
  * -
  */


// const int temperature_sensor_in = A0;

#define DHTTYPE    DHT11     // DHT 11
int get_temperature_value(int pin) {
  DHT dht(pin, DHTTYPE);

  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  } else {
    return temperature;
  }
//   // Read Humidity
//   float newH = dht.readHumidity();
//   // if humidity read failed, don't change h value
//   if (isnan(newH)) {
//     Serial.println("Failed to read from DHT sensor!");
//   }
//   else {
//     h = newH;
// //      Serial.println("HUMIDITY");
// //      Serial.println(h);
//   }
}
