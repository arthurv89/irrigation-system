#include <Arduino.h>
#include "StorageUtil.h"
#include "PermStorageUtil.h"
#include "EEPROMUtil.h"
#include "Utils.h"

PermStorageUtil permStorageUtil;
EEPROMUtil eepromUtil;

int deviceIdLength = 20;

StaticJsonDocument<200> data = emptyJsonObject();

void initializeStorage(bool resetStorage) {
  eepromUtil.initializeEEPROMStorage();
  permStorageUtil.initializePermStorage();

  if(resetStorage) {
    StaticJsonDocument<200> doc = emptyJsonObject();
    data = doc;
    updateEEPROM();
    updatePerm();
  }

  readEEPROMValues();
  if(!data["deviceId"]) {
    // EEPROM not initialised yet.
    readPermStorageValues();
  }

  if(!data["deviceId"]) {
    // Perm storage also nevery initialised.
    // Initialise now.
    data["deviceId"] = createDeviceId();
    data["cycle"] = 0;
    updateEEPROM();
    updatePerm();
  }
}

void copy_value(String key, StaticJsonDocument<200>& from_json, StaticJsonDocument<200>& to_json) {
  if(from_json.containsKey(key)) {
    to_json[key] = from_json[key];
  }
}

void readPermStorageValues() {
  StaticJsonDocument<200> doc = emptyJsonObject();
  DeserializationError error = deserializeJson(doc, permStorageUtil.read_json());
  if(!error) {
    copy_value("deviceId", doc, data);
    copy_value("wifi_ssid", doc, data);
    copy_value("wifi_psk", doc, data);
  }
}

void readEEPROMValues() {
  StaticJsonDocument<200> doc = emptyJsonObject();
  DeserializationError error = deserializeJson(doc, eepromUtil.read_json());
  if(!error) {
    copy_value("deviceId", doc, data);
    copy_value("cycle", doc, data);
    copy_value("wifi_ssid", doc, data);
    copy_value("wifi_psk", doc, data);
  }
  Serial.println(_serializeJson(data));
}

void updateEEPROM() {
  StaticJsonDocument<200> doc = emptyJsonObject();
  copy_value("deviceId", data, doc);
  copy_value("cycle", data, doc);
  copy_value("wifi_ssid", data, doc);
  copy_value("wifi_psk", data, doc);

  eepromUtil.write_json(doc);
}

void updatePerm() {
  Serial.println("Update perm");
  StaticJsonDocument<200> doc = emptyJsonObject();
  copy_value("deviceId", data, doc);
  copy_value("wifi_ssid", data, doc);
  copy_value("wifi_psk", data, doc);

  Serial.println("Writing JSON");
  permStorageUtil.write_json(doc);
}


String createDeviceId() {
  // Create random deviceId
  String deviceIdPostfix = "";
  for (int i=0; i < deviceIdLength; i++) {
    deviceIdPostfix += char(('0' + random(0, 10)));
  }
  return deviceIdPostfix;
}



String getDeviceId() {
  return data["deviceId"];
}

int getCycle() {
  return data["cycle"];
}

void setCycle(int cycle) {
  data["cycle"] = cycle;
  updateEEPROM();
}

boolean containsKey(String key) {
  return data.containsKey(key);
}

String getWifiSsid() {
  Serial.println("Getting WIFI SSID. JSON data = ");
  Serial.println(_serializeJson(data));
  return data["wifi_ssid"];
}

String getWifiPsk() {
  return data["wifi_psk"];
}

void setWifiCredentials(String ssid, String psk) {
  if(data["wifi_ssid"] != ssid || data["wifi_psk"] != psk) {
    data["wifi_ssid"] = ssid;
    data["wifi_psk"] = psk;
    updateEEPROM();
    updatePerm();
  }
}
