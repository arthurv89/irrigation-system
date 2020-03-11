using namespace std;

#include "EEPROMUtil.h"
#include "HandleInstruction.h"
#include "Internet.h"
#include "Request.h"
#include "Utils.h"
#include "BlinkTimes.h"
#include "MoistureSensor.h"

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

const int pin = D2;
const int delay_ms = 3000;

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

  Serial.println("");
  Serial.println("");
  Serial.println("");
  inializeEEPROM(false);

  run();
}


void run() {
  pinMode(pin, INPUT_PULLUP);

  buttonPressed = digitalRead(pin);
  Serial.println("PIN " + String(pin) + " = " + buttonPressed + ", cycle=" + getCycle());

  int cycle = getCycle();
  if(buttonPressed == 1) {
    handle_button_pressed();
  } else if(cycle >= 5 || cycle < 0) {
    setCycle(0);
    do_big_calculation();
  } else {
    no_button_press();
  }
}

void do_big_calculation() {
  Serial.println("Big Calculation");

  long long beforeMillis = millis();
  if(WiFi.SSID().length() > 0) {
    Serial.println("--------------------> Submit data over wifi");
    WiFiManager wifiManager;
    Serial.println(WiFi.SSID().c_str());
    
    
    wifiManager.autoConnect(WiFi.SSID().c_str());
    
    Serial.println();
    Serial.println("==========");
    
    get_settings();
    submit_results();
    digitalWrite(LED_BUILTIN, LOW);
  } else {
      Serial.println("--------------------> Wifi not setup yet. Not doing anything.");
  }
  long long afterMillis = millis();
  deep_sleep(interval, beforeMillis, afterMillis);
}

void handle_button_pressed() {
  // Reboot and handle it once it's rebooted.
  WiFiManager wifiManager;
  Serial.println("Setup wifi");
  wifiManager.resetSettings();
  String wifiName = getWifiName();
  wifiManager.autoConnect(wifiName.c_str());

  Serial.println("Finished setting up wifi");
  sleep(100 * 1000);
}

String getWifiName() {
  return "IRSYS-M-" + getDeviceId();
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
  ESP.deepSleep(us, WAKE_RF_DEFAULT);
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


void loop() {}
