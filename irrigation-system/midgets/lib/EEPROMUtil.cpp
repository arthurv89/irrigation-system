#include <Arduino.h>
#include "EEPROM.h"
#include "EEPROMUtil.h"

const char EEPROMPrefix[] = "IRSYS-EEPROM-PREFIX";

const int cycleStartIdx = strlen(EEPROMPrefix) + 1;

const bool isDebug = false;

void EEPROMUtil::initializeStorage(bool resetEEPROM) {
  if (resetEEPROM || !getPrefixFromStorage().equals(EEPROMPrefix)) {
    printLine("Force initialization of EEPROM");
    writeString("                                                            ", 0);

    writeString(EEPROMPrefix, 0);
    printLine(readStr(0, 50));
    printLine();

    writeInt(0, cycleStartIdx);
    printLine(readStr(0, 50));
  } else {
    printLine("EEPROM was already initialised");
  }
}

int EEPROMUtil::getCycle() {
  return readInt(cycleStartIdx);
}

void EEPROMUtil::setCycle(int cycle) {
  writeInt(cycle, cycleStartIdx);
}

String EEPROMUtil::getPrefixFromStorage() {
  return readStr(0, strlen(EEPROMPrefix));
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

String readStr(int startIndex, int len) {
  char chars[len+1];
  memset(chars, 0, sizeof chars);
  for (int i = 0; i < len; i++) {
    chars[i] = EEPROM.read(startIndex + i);
  }

  chars[len] = '\0';
  return chars;
}

void EEPROMUtil::writeInt(int value, int startpos) {
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

void EEPROMUtil::printLine(String str) {
  if(isDebug) {
    Serial.println(str);
  }
}

void EEPROMUtil::printString(String str) {
  if(isDebug) {
    Serial.print(str);
  }
}

void EEPROMUtil::printString(int i) {
  if(isDebug) {
    Serial.print(i);
  }
}

void EEPROMUtil::printLine() {
  printLine("");
}
