using namespace std;

#include "Arduino.h"
#include <ArduinoJson.h>
#include "Midget.h"
#include "IRunner.h"
#include "TemperatureSensor.h"
#include"DHTLib.h"
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE    DHT11

DHT dht(DHTPIN, DHTTYPE);

int light_sensor_pin = A0;    // select the input pin for the potentiometer

class Runner: public IRunner
{
public:
  int getButtonPin()
  {
    return D2;
  }

  String getWifiName(String deviceId) {
    return "IRSYS-T-" + deviceId;
  }

  void add_sensor_values(StaticJsonDocument<200> &doc) {
    doc["temperature"] = get_temperature_value(dht);
    doc["light"] = analogRead(light_sensor_pin);
  }
};


void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);
  dht.begin();
}


void loop() {
    loopMidget();
}
