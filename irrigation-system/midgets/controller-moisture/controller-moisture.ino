using namespace std;

#include "Arduino.h"
#include "Midget.h"
#include "IRunner.h"
#include "MoistureSensor.h"

class Runner: public IRunner
{
public:
  int getPin()
  {
    return D2;
  }

  String getWifiName(String deviceId) {
    return "IRSYS-M-" + deviceId;
  }

  String getValue() {
    return get_moisture_value() + "";
  }
};


void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);
}


void loop() {}