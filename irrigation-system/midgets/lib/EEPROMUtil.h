using namespace std;

#include <ArduinoJson.h>

#ifndef EEPROM_UTIL_H
#define EEPROM_UTIL_H
class EEPROMUtil {
  public:
    virtual int getCycle() = 0;
    virtual void setCycle(int cycle);
    virtual void initializeStorage(boolean resetStorage) ;

  private:
    virtual String getDeviceId() = 0;
    virtual String getPrefixFromStorage() = 0;
    virtual void writePermStorage(int cycle) ;
    virtual void writeString(String str, int startpos);
    virtual String readStr(int startIndex, int len) = 0;
    virtual void writeInt(int cycle, int startpos);
    virtual int readInt(int startpos) = 0;
    virtual String createDeviceId() = 0;
    virtual void printLine(String str);
    virtual void printString(String str);
    virtual void printString(int i);
    virtual void printLine();
};
#endif
