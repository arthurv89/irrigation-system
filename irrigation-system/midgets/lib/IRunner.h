using namespace std;

#include "Arduino.h"
#include "ArduinoJson.h"

#ifndef IRUNNER_H
#define IRUNNER_H
class IRunner {
  public:
    virtual int getButtonPin() = 0;
    virtual String getWifiName(String deviceId) = 0;
    virtual void add_sensor_values(StaticJsonDocument<200>& doc);
};
#endif
