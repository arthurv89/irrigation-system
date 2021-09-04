#include <Arduino.h>
#include "EEPROM.h"
#include "EEPROMUtil.h"
#include "Utils.h"

void EEPROMUtil::initializeEEPROMStorage() {
  EEPROM.begin(512);
}


void EEPROMUtil::write_json(StaticJsonDocument<2048> doc) {
  String str = _serializeJson(doc);
  Serial.println("Writing contents to EEPROM: " + str);

  writeInt(0, str.length());

  int startPos = 2;
  for (int i=0; i < str.length(); i++) {
    EEPROM.write(i + startPos, str[i]);
  }
  EEPROM.commit();
}

String EEPROMUtil::read_json() {
  Serial.println("Reading EEPROM");
  int len = readInt(0);
  if(len > 512) {
    return "ERROR!!!";
  }

  int startPos = 2;

  char chars[len+1];
  memset(chars, 0, sizeof chars);
  for (int i = 0; i < len; i++) {
    chars[i] = EEPROM.read(startPos + i);
  }

  chars[len] = '\0';
  return chars;
}


void EEPROMUtil::writeInt(int p_address, int p_value) {
   byte lowByte = ((p_value >> 0) & 0xFF);
   byte highByte = ((p_value >> 8) & 0xFF);

   EEPROM.write(p_address, lowByte);
   EEPROM.write(p_address + 1, highByte);
   EEPROM.commit();
 }

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
int EEPROMUtil::readInt(int p_address) {
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
