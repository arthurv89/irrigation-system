using namespace std;

#include "Arduino.h"

#ifndef IRUNNER_H
#define IRUNNER_H
class IRunner {
  public:
    virtual int getButtonPin() = 0;
    virtual String getWifiName(String deviceId) = 0;
    virtual String getKey() = 0;
    virtual float getValue() = 0;
};
#endif
