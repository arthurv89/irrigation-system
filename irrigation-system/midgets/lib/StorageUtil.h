#include <Arduino.h>
#include <ArduinoJson.h>

void initializeStorage(bool resetStorage);
String getDeviceId();
int getCycle();
void setCycle(int cycle);
void setWifiCredentials(String ssid, String psk);
String getWifiSsid();
String getWifiPsk();
boolean containsKey(String key);

// Private
void copy_value(String key, StaticJsonDocument<2048>& from_json, StaticJsonDocument<2048>& to_json);
void readEEPROMValues();
void readPermStorageValues();
String createDeviceId();
void updateEEPROM();
void updatePerm();
