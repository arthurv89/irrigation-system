#include "Arduino.h"
#include "ArduinoJson.h"
#include "LiquidCrystal_I2C.h"

#ifndef IRUNNER_H
#define IRUNNER_H
class IRunner {
  public:
    virtual String getType() = 0;
    virtual int getButtonPin() = 0;
    virtual int getValvePin(int valve) = 0;
    virtual String getWifiName(String deviceId) = 0;
    virtual LiquidCrystal_I2C* getLcd();

    virtual bool setup_wifi() = 0;

    virtual void add_sensor_values(JsonObject& doc);

};
#endif
