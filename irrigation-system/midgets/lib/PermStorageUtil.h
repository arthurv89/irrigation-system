using namespace std;

#include <ArduinoJson.h>

#ifndef PERM_STORAGE_UTIL_H
#define PERM_STORAGE_UTIL_H
class PermStorageUtil {
  public:
    virtual void initializeStorage(bool resetPermStorage);
    virtual String getDeviceId() = 0;

  private:
    virtual String createDeviceId() = 0;
    virtual void write_json(StaticJsonDocument<200> doc);
    virtual StaticJsonDocument<200> read_json();

    StaticJsonDocument<200> json;
};
#endif
