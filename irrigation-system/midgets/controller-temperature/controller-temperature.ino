using namespace std;

#include "Arduino.h"
#include "Midget.h"
#include "IRunner.h"
#include "TemperatureSensor.h"
#include"DHTLib.h"
#include <DHT.h>

#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

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

  String getKey() {
    return "temperature";
  }

  float getValue() {
    return get_temperature_value(dht);
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
