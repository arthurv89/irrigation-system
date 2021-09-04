using namespace std;

#include <ArduinoJson.h>

#ifndef PERM_STORAGE_UTIL_H
#define PERM_STORAGE_UTIL_H
class PermStorageUtil {
  public:
    void initializePermStorage();
    String read_json();
    void write_json(StaticJsonDocument<2048> doc);
};
#endif
