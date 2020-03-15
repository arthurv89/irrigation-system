using namespace std;

#include "Arduino.h"
#include "Midget.h"
#include "IRunner.h"
#include "TemperatureSensor.h"
#include"DHTLib.h"

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
    return get_temperature_value(D1);
  }
};


void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);
  dhtlib_init();
}


void loop() {
    loopMidget();
}
