using namespace std;

#include <ArduinoJson.h>

#ifndef EEPROM_UTIL_H
#define EEPROM_UTIL_H
class EEPROMUtil {
  private:
    int getCycle();
    void setCycle(int cycle);
    String getPrefixFromPermStorage() ;
    void writeStorage(int cycle) ;
    void inializeStorage(boolean resetDeviceId) ;
    void writeString(String str, int startpos) ;
    String readStr(int startIndex, int len);
    void writeInt(int cycle, int startpos);
    int readInt(int startpos);
    void printLine(String str);
    void printString(String str);
    void printString(int i);
    void printLine();
};
#endif
