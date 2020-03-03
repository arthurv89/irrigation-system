using namespace std;

#include "DeviceId.h"
#include "HandleInstruction.h"
#include "Internet.h"
#include "Request.h"
#include "Utils.h"
#include "BlinkTimes.h"

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager


boolean buttonPressed;

// Loop variables
long long previousMillis = 0;
const int interval = 3000; // Should be 300000 (5 minutes)


/*********************************************
                     Setup
 *********************************************/

WiFiServer server(80);
String header;

// Settings URL can be changed to a static file in S3 (as long as we can find the settings for this specific owner)
String settings_url = "http://192.168.1.3:8123/api/get-settings";

void setup() {
  _setup();
}

void _setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  setDeviceId();
  String deviceId = getDeviceId();
  Serial.print("Device id: ");
  Serial.println(deviceId);

  
  buttonPressed = isButtonReset();
  if(buttonPressed) {
    handleReset();
  } else {
    handleWakeup();
  }

  delay(1000);
}

boolean isButtonReset() {
  return ESP.getResetInfoPtr()->reason == 6;
}

void handleWakeup() {
  Serial.println("Wake up!");
}

void handleReset() {
  Serial.println("Button pressed");
}



void run() {
  Serial.println("Run");
//  Serial.println(WiFi.SSID());
//  Serial.println(WiFi.psk());
  WiFiManager wifiManager;

  if(buttonPressed) { // Wifi reset
//    blink_count = 5;
    wifiManager.resetSettings();
  } else {
//    blink_count = 6;
  }
//  blink_reverse(LED_BUILTIN, blink_count);

  long long beforeMillis = millis();
//  perform_action();
  long long afterMillis = millis();

  if(buttonPressed) {
    String wifiName = getDeviceId();
    wifiManager.autoConnect(wifiName.c_str());
  } else {
    wifiManager.autoConnect(WiFi.SSID().c_str());
    perform_action();
    digitalWrite(LED_BUILTIN, LOW);
    deep_sleep(interval, beforeMillis, afterMillis);
  }
  buttonPressed = false;
}

void deep_sleep(int interval, long long beforeMillis, long long afterMillis) {
  long long delay_time_millis = max(1LL, interval - (afterMillis - beforeMillis));

  Serial.println("Sleeping for some time: " + to_str(delay_time_millis));
  Serial.println(String(interval));
  Serial.println(to_str(beforeMillis));
  Serial.println(to_str(afterMillis));

  delay(100);
  ESP.deepSleep( delay_time_millis * 1000, WAKE_RF_DEFAULT);
}


void perform_action() {
  Serial.println();
  Serial.println("==========");

  get_settings();
  submit_results();
}

StaticJsonDocument<200> settings;
void get_settings() {
  String response = do_get_request(settings_url);
  Serial.println(response);
  
  settings = deserializeJson(response)["response"];
  String controller_ip = settings["controller_ip"];
  Serial.println("controller_ip");
  Serial.println(controller_ip);

  String payload;
  serializeJson(settings, payload);
  Serial.println(payload);

//  settings["controller_ip"] = "192.168.1.3:8123";
}


int get_moisture_value() {
    return random(10, 20);
}


void submit_results() {
  String ip = settings["controller_ip"];
  String url = "http://" + ip + "/api/submit";
  String payload;
  StaticJsonDocument<200> doc;
  doc["deviceId"] = getDeviceId();
  doc["moisture"] = get_moisture_value();
  serializeJson(doc, payload);

  Serial.println("Payload");
  Serial.println(payload);

  String get_response = do_post_request(url, payload);

  Serial.println("Response for url ");
  Serial.println(url);
  Serial.println(get_response);
}


void loop() {
  run();

  delay(300);
}
