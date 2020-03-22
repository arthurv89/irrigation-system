/**
 * How to install:
 * Assuming relay is with the blue side on top, and you're facing the pins.
 *
 * [XXX, Common, +]
 * [__________]
 * |          |
 * | BLUE BOX |
 * [__________]
 *  |    |   |
 * [D1, 3V, GND]:
 *
 */

using namespace std;

#include "Midget.h"
#include "IRunner.h"
#include "Valve.h"

const int pins = 3;
const int valvePins[pins] = {D1, D2, D3};


class Runner: public IRunner
{
public:
  boolean setup_wifi() {
    return false;
  }

  String getType() {
    return "valve";
  }

  int getButtonPin() {
    return D8;
  }

  String getWifiName(String deviceId) {
    return "IRSYS-V-" + deviceId;
  }

  void add_sensor_values(JsonObject &doc) {
    handle(this);
  }

  int getValvePin(int valve) {
    return valvePins[valve];
  }
};


void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);

  for(int p=0; p<pins; p++) {
    int pin = valvePins[p];
    pinMode(pin, OUTPUT);
  }
}

void loop() {
    loopMidget();
}
