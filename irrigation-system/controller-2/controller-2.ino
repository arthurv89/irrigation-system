using namespace std;

#include "DeviceId.h"
#include "HandleInstruction.h"
#include "Internet.h"
#include "Request.h"
#include "Utils.h"

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>

// Loop variables
long long previousMillis = 0;
const int interval = 1000; // Should be 300000 (5 minutes)


/*********************************************
                     Setup
 *********************************************/

void setup() {
  _setup();
}

void _setup() {
}



void run() {
  
  Serial.begin(115200);
  EEPROM.begin(512);

  Serial.println("LEDs");
  Serial.println(D0);
  Serial.println(D1);
  Serial.println(D2);
  Serial.println(D3);
  Serial.println(D4);
  Serial.println(D5);
  Serial.println(D6);
  Serial.println(D7);

  long long beforeMillis = millis();
  Serial.println();
  Serial.println("[Setup] Starting up");


  setDeviceId();
  String deviceId = getDeviceId();
  Serial.print("Device id: ");
  Serial.println(deviceId);

  perform_action();
  long long afterMillis = millis();

  long long delay_time_millis = max(1LL, interval - (afterMillis - beforeMillis));
//  long long delay_time_millis = 1000;

  // Avoid sleeping forever
  Serial.print("Sleeping for some time: ");
  Serial.println(to_str(delay_time_millis));
//  ESP.deepSleep( delay_time_millis * 1000, WAKE_RF_DEFAULT);
}


int x = 0;

void perform_action() {
  Serial.println();
  Serial.println("==========");

//  print_page("");

  
//  connect_wifi();
//  print_page("http://jigsaw.w3.org/HTTP/connection.html");

  connect_wifi();
  print_page("http://192.168.1.3:8000/submit");
  




void loop() {
  run();

  delay(300);
}
