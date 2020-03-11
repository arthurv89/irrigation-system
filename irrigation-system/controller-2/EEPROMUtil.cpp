#include <Arduino.h>
#include "EEPROMUtil.h"
#include "EEPROM.h"

const int deviceIdLength = 20;
const char eepromPrefix[] = "IRSYS-EEPROM-PREFIX";

const int deviceIdStartIdx = strlen(eepromPrefix) + 1;
const int cycleStartIdx = strlen(eepromPrefix) + 1 + deviceIdLength + 1;

const bool isDebug = false;

void inializeEEPROM(bool resetEEPROM) {
  if (resetEEPROM || !getPrefixFromEEPROM().equals(eepromPrefix)) {
    printLine("Force initialization of EEPROM");
    writeString("                                                            ", 0);

    String deviceId = createDeviceId();
    printLine("deviceId: " + deviceId);
    printLine();

    writeString(eepromPrefix, 0);
    printLine(readStr(0, 50));
    printLine();

    writeString(deviceId, deviceIdStartIdx);
    printLine(readStr(0, 50));
    printLine();

    writeInt(0, cycleStartIdx);
    printLine(readStr(0, 50));
  } else {
    printLine("EEPROM was already initialised");
  }
}

String getDeviceId() {
  return readStr(deviceIdStartIdx, deviceIdLength);
}

int getCycle() {
  return readInt(cycleStartIdx);
}

void setCycle(int cycle) {
  writeInt(cycle, cycleStartIdx);
}

String getPrefixFromEEPROM() {
  return readStr(0, strlen(eepromPrefix));
}

String createDeviceId() {
  // Create random deviceId
  String deviceIdPostfix = "";
  for (int i=0; i < deviceIdLength; i++) {
    deviceIdPostfix += char(('0' + random(0, 10)));
    printLine(deviceIdPostfix);
  }
  return deviceIdPostfix;
}

void writeString(String str, int startpos) {
  printString("WRITING ON POSITION ");
  printString(startpos);
  printString(" [len=");
  printString(str.length());
  printString("]: ");
  for (int i=0; i < str.length(); i++) {
    EEPROM.write(i + startpos, str[i]);
  }
  EEPROM.commit();
  printLine(str);
}

String readStr(int startIndex, int len) {
  char chars[len+1];
  memset(chars, 0, sizeof chars);
  for (int i = 0; i < len; i++) {
    chars[i] = EEPROM.read(startIndex + i);
  }

  chars[len] = '\0';
  return chars;
}

void writeInt(int value, int startpos) {
  printString("WRITING ON POSITION ");
  printString(startpos);
  printString(": ");
  printString(value + '0');
  printLine();
  EEPROM.write(startpos, value + '0');
  EEPROM.commit();
}

int readInt(int startpos) {
  return EEPROM.read(startpos) - '0';
}

void printLine(String str) {
  if(isDebug) {
    Serial.println(str);
  }
}

void printString(String str) {
  if(isDebug) {
    Serial.print(str);
  }
}

void printString(int i) {
  if(isDebug) {
    Serial.print(i);
  }
}

void printLine() {
  printLine("");
}
