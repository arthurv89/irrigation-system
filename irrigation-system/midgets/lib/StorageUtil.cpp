#include "StorageUtil.h"
#include "PermStorageUtil.h"
#include "EEPROMUtil.h"
#include "Utils.h"

PermStorageUtil permStorageUtil;
EEPROMUtil eepromUtil;

int deviceIdLength = 20;

StaticJsonDocument<2048> response_data = _emptyJsonObject();

void initializeStorage(bool resetStorage) {
  eepromUtil.initializeEEPROMStorage();
  permStorageUtil.initializePermStorage();

  if(resetStorage) {
    StaticJsonDocument<2048> doc = _emptyJsonObject();
    response_data = doc;
    updateEEPROM();
    updatePerm();
  }

  readEEPROMValues();
  if(!response_data["deviceId"]) {
    // EEPROM not initialised yet.
    readPermStorageValues();
  }

  if(!response_data["deviceId"]) {
    // Perm storage also nevery initialised.
    // Initialise now.
    response_data["deviceId"] = createDeviceId();
    response_data["cycle"] = 0;
    updateEEPROM();
    updatePerm();
  }
}

void copy_value(String key, StaticJsonDocument<2048>& from_json, StaticJsonDocument<2048>& to_json) {
  if(from_json.containsKey(key)) {
    to_json[key] = from_json[key];
  }
}

void readPermStorageValues() {
  StaticJsonDocument<2048> doc = _emptyJsonObject();
  DeserializationError error = deserializeJson(doc, permStorageUtil.read_json());
  if(!error) {
    copy_value("deviceId", doc, response_data);
    copy_value("wifi_ssid", doc, response_data);
    copy_value("wifi_psk", doc, response_data);
  }
}

void readEEPROMValues() {
  StaticJsonDocument<2048> doc = _emptyJsonObject();
  DeserializationError error = deserializeJson(doc, eepromUtil.read_json());
  if(!error) {
    copy_value("deviceId", doc, response_data);
    copy_value("cycle", doc, response_data);
    copy_value("wifi_ssid", doc, response_data);
    copy_value("wifi_psk", doc, response_data);
  }
  Serial.println(_serializeJson(response_data));
}

void updateEEPROM() {
  StaticJsonDocument<2048> doc = _emptyJsonObject();
  copy_value("deviceId", response_data, doc);
  copy_value("cycle", response_data, doc);
  copy_value("wifi_ssid", response_data, doc);
  copy_value("wifi_psk", response_data, doc);

  eepromUtil.write_json(doc);
}

void updatePerm() {
  Serial.println("Update perm");
  StaticJsonDocument<2048> doc = _emptyJsonObject();
  copy_value("deviceId", response_data, doc);
  copy_value("wifi_ssid", response_data, doc);
  copy_value("wifi_psk", response_data, doc);

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
  return response_data["deviceId"];
}

int getCycle() {
  return response_data["cycle"];
}

void setCycle(int cycle) {
  response_data["cycle"] = cycle;
  updateEEPROM();
}

boolean containsKey(String key) {
  return response_data.containsKey(key);
}

String getWifiSsid() {
  Serial.println("Getting WIFI SSID. JSON response_data = ");
  Serial.println(_serializeJson(response_data));
  return response_data["wifi_ssid"];
}

String getWifiPsk() {
  return response_data["wifi_psk"];
}

void setWifiCredentials(String ssid, String psk) {
  if(response_data["wifi_ssid"] != ssid || response_data["wifi_psk"] != psk) {
    response_data["wifi_ssid"] = ssid;
    response_data["wifi_psk"] = psk;
    updateEEPROM();
    updatePerm();
  }
}
