using namespace std;

#include <ArduinoJson.h>

#ifndef PERM_STORAGE_UTIL_H
#define PERM_STORAGE_UTIL_H
class PermStorageUtil {
  public:
    void initializePermStorage(bool resetPermStorage);
    String read_json();
    void write_json(StaticJsonDocument<200> doc);
};
#endif
