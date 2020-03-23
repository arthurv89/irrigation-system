using namespace std;

#include <ArduinoJson.h>

#ifndef EEPROM_UTIL_H
#define EEPROM_UTIL_H
class EEPROMUtil {
  public:
    void initializeEEPROMStorage(bool resetStorage);
    void write_json(StaticJsonDocument<200> doc);
    String read_json();

  private:
    void writeInt(int p_address, int p_value);
    int readInt(int startPos);
};
#endif
