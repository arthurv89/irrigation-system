using namespace std;

#include "Arduino.h"
#include "Midget.h"
#include "IRunner.h"
#include "MoistureSensor.h"

boolean setup_wifi = false;

class Runner: public IRunner
{
public:
  boolean setup_wifi() {
    return setup_wifi;
  }

  String getType() {
    return "moisture";
  }

  int getButtonPin() {
    return D2;
  }

  String getWifiName(String deviceId) {
    return "IRSYS-M-" + deviceId;
  }

  void add_sensor_values(JsonObject &doc) {
    doc["moisture"] = get_moisture_value();
  }
};


void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);
}


void loop() {
    loopMidget();
}
