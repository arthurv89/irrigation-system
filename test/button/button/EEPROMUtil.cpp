#include <Arduino.h>
#include "EEPROMUtil.h"
#include "EEPROM.h"

const int deviceIdLength = 20;
const char PermStoragePrefix[] = "IRSYS-PermStorage-PREFIX";

const int deviceIdStartIdx = strlen(PermStoragePrefix) + 1;
const int cycleStartIdx = strlen(PermStoragePrefix) + 1 + deviceIdLength + 1;

const bool isDebug = false;

void EEPROMUtil::initializeStorage(bool resetPermStorage) {
  if (resetPermStorage || !getPrefixFromPermStorage().equals(PermStoragePrefix)) {
    printLine("Force initialization of PermStorage");
    writeString("                                                            ", 0);

    writeString(PermStoragePrefix, 0);
    printLine(readStr(0, 50));
    printLine();

    writeString(deviceId, deviceIdStartIdx);
    printLine(readStr(0, 50));
    printLine();

    writeDigit(0, cycleStartIdx);
    printLine(readStr(0, 50));
  } else {
    printLine("PermStorage was already initialised");
  }
}

String EEPROMUtil::getDeviceId() {
  return readStr(deviceIdStartIdx, deviceIdLength);
}

int EEPROMUtil::getCycle() {
  return readInt(cycleStartIdx);
}

void EEPROMUtil::setCycle(int cycle) {
  writeDigit(cycle, cycleStartIdx);
}

String EEPROMUtil::getPrefixFromPermStorage() {
  return readStr(0, strlen(PermStoragePrefix));
}

String EEPROMUtil::createDeviceId() {
  // Create random deviceId
  String deviceIdPostfix = "";
  for (int i=0; i < deviceIdLength; i++) {
    deviceIdPostfix += char(('0' + random(0, 10)));
    printLine(deviceIdPostfix);
  }
  return deviceIdPostfix;
}

void EEPROMUtil::writeString(String str, int startpos) {
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

String EEPROMUtil::readStr(int startPos, int len) {
  char chars[len+1];
  memset(chars, 0, sizeof chars);
  for (int i = 0; i < len; i++) {
    chars[i] = EEPROM.read(startPos + i);
  }

  chars[len] = '\0';
  return chars;
}

void EEPROMUtil::writeDigit(int value, int startpos) {
  printString("WRITING ON POSITION ");
  printString(startpos);
  printString(": ");
  printString(value + '0');
  printLine();
  EEPROM.write(startpos, value + '0');
  EEPROM.commit();
}

int EEPROMUtil::readInt(int startpos) {
  return EEPROM.read(startpos) - '0';
}
