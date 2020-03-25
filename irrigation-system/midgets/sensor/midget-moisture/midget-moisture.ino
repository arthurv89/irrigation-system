using namespace std;

#include "Arduino.h"
#include "Midget.h"
#include "IRunner.h"
#include "MoistureSensor.h"

const int moisture_sensor_in = A0;
const int pin1 = D3;
const int pin2 = D4;

class Runner: public IRunner
{
public:
  int getValvePin(int i) {
    return -1;
  }

  bool setup_wifi() {
    return false;
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
    doc["moisture"] = get_moisture_value(moisture_sensor_in, pin1, pin2);
  }
};


void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);
}


void loop() {
    loopMidget();
}
