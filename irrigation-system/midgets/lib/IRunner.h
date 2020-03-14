using namespace std;

#include "Arduino.h"

#ifndef IRUNNER_H
#define IRUNNER_H
class IRunner {
  public:
    virtual int getPin() = 0;
    virtual String getWifiName(String deviceId) = 0;
    virtual String getValue() = 0;
};
#endif
