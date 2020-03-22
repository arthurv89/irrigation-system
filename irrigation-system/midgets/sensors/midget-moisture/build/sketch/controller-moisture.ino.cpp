#line 1 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino"
#line 1 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino"
using namespace std;

#include "Arduino.h"
#include "Midget.h"
#include "IRunner.h"
#include "MoistureSensor.h"

class Runner: public IRunner
{
public:
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


#line 29 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino"
void setup();
#line 35 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino"
void loop();
#line 29 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino"
void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);
}


void loop() {
    loopMidget();
}

