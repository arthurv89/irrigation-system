using namespace std;

#include "Arduino.h"
#include "Midget.h"
#include "IRunner.h"
#include "TemperatureSensor.h"
#include"DHTLib.h"

class Runner: public IRunner
{
public:
  int getPin()
  {
    return D2;
  }

  String getWifiName(String deviceId) {
    return "IRSYS-T-" + deviceId;
  }

  String getValue() {
    return get_temperature_value() + "";
  }
};


void setup() {
  dhtlib_init ()

  IRunner* runner = new Runner;
  setupMidget(runner);
}


void loop() {}
