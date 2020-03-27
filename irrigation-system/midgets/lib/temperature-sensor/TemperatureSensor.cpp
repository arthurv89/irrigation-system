#include "Arduino.h"
#include "TemperatureSensor.h"
#include "DHTLib.h"
#include "DHTReading.h"

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


#define DHTTYPE    DHT11     // DHT 11

DHTReading getDHTMeasurement(DHT dht) {
  DHTReading reading;

  float temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    reading.temperature = -1;
  } else {
    reading.temperature = temperature;
  }

  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed to read humidity from DHT sensor!");
    reading.humidity = -1;
  } else {
    reading.humidity = humidity;
  }

  return reading;
}
