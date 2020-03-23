using namespace std;

#include <ArduinoJson.h>

#ifndef EEPROM_UTIL_H
#define EEPROM_UTIL_H
class EEPROMUtil {
  public:
    void initializeStorage(bool resetEEPROM);
    void write_json(String str);
    String read_json(int startPos, int len);
};
#endif
