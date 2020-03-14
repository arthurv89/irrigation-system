using namespace std;

#include "lib/IRunner.cpp"
#include "lib/Midget.cpp"
#include "lib/MoistureSensor.h"

const int pin = D2;

IRunner runner;

void setup() {
  _setup(runner);
}


void run() {
  _run();
}


void loop() {}

class Runner: public IRunner
{
public:
  int getPin()
  {
    return pin;
  }

  String getWifiName() {
    return "IRSYS-M-" + getDeviceId();
  }

  String getValue() {
    return get_moisture_value();
  }
};
