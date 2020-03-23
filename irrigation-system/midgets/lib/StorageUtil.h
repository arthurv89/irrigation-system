void initializeStorage(bool resetStorage);
String getDeviceId();
int getCycle();
void setCycle(int cycle);
void setWifiCredentials(String ssid, String psk);
String getWifiSsid();
String getWifiPsk();

// Private
void readEEPROMValues();
void readPermStorageValues();
String createDeviceId();
void updateEEPROM();
void updatePerm();
