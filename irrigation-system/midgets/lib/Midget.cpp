#include "Midget.h"
#include "IRunner.h"
#include "Pin.h"
#include "Internet.h"
#include "StorageUtil.h"
#include "Request.h"
#include "Utils.h"
#include "BlinkTimes.h"

#include <ArduinoJson.h> // https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"         // https://github.com/tzapu/WiFiManager
#include "ESP8266httpUpdate.h"


bool buttonPressed;

// Loop variables
long long previousMillis = 0;
const int interval = 3000; // Should be 300000 (5 minutes)

const int delay_ms = 3000;

/*********************************************
                     Setup
 *********************************************/

WiFiServer server(80);
String header;

IRunner* iRunner;

// Settings URL can be changed to a static file in S3 (as long as we can find the settings for this specific owner)
String settings_url = "http://192.168.1.192:8123/api/v2/get-settings";
StaticJsonDocument<200> settings = emptyJsonObject();
WiFiClient wiFiClient;

void setupMidget(IRunner* _iRunner) {
    iRunner = _iRunner;
    Serial.begin(115200);

    Serial.println("");
    Serial.println("");
    Serial.println("");
    initializeStorage(false);
}

void loopMidget() {
    pinMode(iRunner->getButtonPin(), INPUT_PULLUP);

    buttonPressed = false;
    // buttonPressed = digitalRead(iRunner->getButtonPin()) == 0;
    Serial.println("PIN " + String(iRunner->getButtonPin()) + " = " + String(buttonPressed) + ", cycle=" + getCycle());

    int cycle = getCycle();
    if(!is_wifi_setup()) {
      Serial.println("Wifi not yet set up");
      setup_wifi();
    } else {
      Serial.println("Wifi already set up");
      if(buttonPressed) {
        Serial.println("Button pressed");
        setup_wifi();
      } else if(cycle >= 5 || cycle < 0) {
        Serial.println("Cycle: " + String(cycle));
        setCycle(0);
        do_big_calculation();
      } else {
        no_button_press();
      }
    }
}

boolean is_wifi_setup() {
  String ssid = getWifiSsid();
  String psk = getWifiPsk();

  Serial.println("WIFI credentials");
  Serial.println(ssid);
  Serial.println(psk);

  return containsKey("wifi_ssid") && !(ssid == NULL) && ssid && ssid.length() > 0;
}

void do_big_calculation() {
  Serial.println("Big Calculation");
  digitalWrite(LED_BUILTIN, HIGH);

  long long beforeMillis = millis();

  connect_wifi();

  fetch_settings();

  String payload = create_payload();
  submit_results(payload);
  digitalWrite(LED_BUILTIN, LOW);
  update_code();
  long long afterMillis = millis();
  deep_sleep(interval, beforeMillis, afterMillis);
}

void update_code() {
  ESPhttpUpdate.rebootOnUpdate(false);
  String ip = settings["controller_addr"]["ip"];
  int port = settings["controller_addr"]["port"];
  Serial.println("Getting new bin: " + ip + ":" + port + "/bin/" + iRunner->getType() + "?deviceId=" + getDeviceId());
  int update_res = ESPhttpUpdate.update(wiFiClient, ip + ":" + port + "/bin/" + iRunner->getType() + "?deviceId=" + getDeviceId());
  Serial.println(update_res);

  switch(update_res) {
      case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
//          Serial.println(binFile);
          break;

      case HTTP_UPDATE_NO_UPDATES:
          Serial.println("HTTP_UPDATE_NO_UPDATES");
          break;

      case HTTP_UPDATE_OK:
          Serial.println("HTTP_UPDATE_OK");
          break;
  }
}

String create_payload() {
  StaticJsonDocument<200> doc = emptyJsonObject();

  JsonObject meta  = doc.createNestedObject("meta");
  meta["heapFreeMem"] = ESP.getFreeHeap();

  JsonObject params  = doc.createNestedObject("params");
  iRunner->add_sensor_values(params);
  params["deviceId"] = getDeviceId();

  String payload = _serializeJson(doc);
  return payload;
}

void no_button_press() {
  // Just wait for the next cycle to see if a button is pressed
  Serial.println("No button press");
  sleep(delay_ms * 1000);
}


void deep_sleep(int interval, long long beforeMillis, long long afterMillis) {
  long long delay_time_millis = max(1LL, interval - (afterMillis - beforeMillis));

  Serial.println("Sleeping for some time: " + to_str(delay_time_millis));
  Serial.println(String(interval));
  Serial.println(to_str(beforeMillis));
  Serial.println(to_str(afterMillis));

  sleep(delay_time_millis * 1000);
}

void sleep(int us) {
  setCycle(getCycle() + 1);
  Serial.println("Sleeping for " + String(us) + " microseconds");
  delay(1000);
  ESP.deepSleep(us, WAKE_RF_DEFAULT);
}

void fetch_settings() {
  String response = do_get_request(settings_url);
  Serial.println(response);

  settings = _deserializeJson(response)["response"];

  String payload;
  serializeJson(settings, payload);
  Serial.println(payload);
}

void submit_results(String payload) {
  Serial.println("--------------------> Submit data over wifi");
  Serial.println("Data: " + payload);

  String ip = settings["controller_addr"]["ip"];
  int port = settings["controller_addr"]["port"];
  String controller_addr = ip + ":" + port;
  String url = "http://" + controller_addr + "/api/submit";

  String get_response = do_post_request(url, payload);

  Serial.println("Response for url ");
  Serial.println(url);
  Serial.println(get_response);
}

void setup_wifi() {
  // Reboot and handle it once it's rebooted.
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  String wifiName = iRunner->getWifiName(getDeviceId());

  Serial.println("Setup wifi. WifiName: " + wifiName);
  wifiManager.startConfigPortal(wifiName.c_str());
  // wifiManager.autoConnect(wifiName.c_str());

  Serial.println("Finished setting up wifi. Storing in Perm storage");
  setWifiCredentials(WiFi.SSID(), WiFi.psk());
  sleep(100 * 1000);
}

StaticJsonDocument<200> get_settings() {
  return settings;
}
