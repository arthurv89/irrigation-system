using namespace std;

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

//static const uint8_t D0   = 16;
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4;
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2;
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;
//static const uint8_t D10  = 1;

// Loop variables
long long previousMillis = 0;
const int interval = 300000; // Should be 300000 (5 minutes)


// WIFI
#ifndef STASSID"GET"

#define STASSID "NOS-172E"
#define STAPSK  "TVZY3KNX"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;


// AWS endpoint
const char* host = "gktyevilwf.execute-api.us-east-1.amazonaws.com";


const String owner = "casabatata";
const boolean resetDeviceId = false;
const char prefix[] = "IRSYS-";
const int deviceIdLength = 20;
const int wateringThreshold = 500;

/*********************************************
                     Setup
 *********************************************/

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

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
  ESP.deepSleep( delay_time_millis * 1000, WAKE_RF_DEFAULT);
}

String getDeviceId() {
  char deviceId[deviceIdLength+1];
  memset(deviceId, 0, sizeof deviceId);
  for (int i = 0; i < deviceIdLength; i++) {
    deviceId[i] = EEPROM.read(i);
  }
  deviceId[deviceIdLength] = '\0';
//  String deviceId = "IRSYS-66320771887196";
  return deviceId;
}

String getPrefix(String s) {
  return s.substring(0, strlen(prefix));
}

/**
   Only sets it if we don't have a deviceId yet!
*/
void setDeviceId() {
  String deviceId = getDeviceId();
  Serial.print("DeviceId: ");
  Serial.println(deviceId);
  if (resetDeviceId || !getPrefix(deviceId).equals(prefix)) {
    Serial.println("Writing back deviceId to EEPROM");

    // Create random deviceId and write it back to EEPROM
    for (int i = 0; i < strlen(prefix); i++) {
      EEPROM.write(i, prefix[i]);
      delay(100);
    }
    for (int i = strlen(prefix); i < deviceIdLength; i++) {
      char x = '0' + random(0, 10);
      EEPROM.write(i, x);
      delay(100);
    }
    EEPROM.commit();
    for (int i = 0; i < deviceIdLength; i++) {
      char y = (char) EEPROM.read(i);
      Serial.print(y);
    }
    Serial.println("");
  } else {
    Serial.println("DeviceId already set");
  }
}



int x = 0;

void perform_action() {
  Serial.println();
  Serial.println("==========");
  
  connect_wifi();
  String response = do_get_request(owner);
  x = 1-x;
//  String response = getStaticResponse(x);
  Serial.println("Response:");
  Serial.println(response);
  
  DynamicJsonDocument jsonObj = deserialize(response);
  handle(jsonObj);
}

String getStaticResponse(int x) {
  return "{"
    "\"instructions\":["
      "{"
        "\"function\": \"dw\","
        "\"params\": {"
          "\"pin\": 4,"
          "\"value\": " + to_str(x) + ""
        "}"
      "},{"
        "\"function\": \"d\","
        "\"params\": {"
          "\"ms\": 1000"
        "}"
      "},{"
        "\"function\": \"dw\","
        "\"params\": {"
          "\"pin\": 4,"
          "\"value\": " + to_str(x) + ""
        "}"
      "}"
    "]"
  "}";
}


void handleDigitalWriteInstruction(uint8_t pinNo, int iVal) {
  int val = iVal == 1 ? HIGH : LOW;
  Serial.print("Set pin ");
  Serial.print(pinNo);
  Serial.print(" to ");
  Serial.println(val);
  pinMode(pinNo, OUTPUT);
  digitalWrite(pinNo, val);
}

void handleDelayInstruction(int ms) {
  Serial.print("Delay ");
  Serial.println(ms);
  delay(ms);
}


void handle(DynamicJsonDocument& jsonObj) {
  JsonArray instructions = jsonObj["instructions"].as<JsonArray>();
  for(auto instruction : instructions) {
    String funcName = instruction["function"].as<String>();
    JsonObject params = instruction["params"];
    handleInstruction(funcName, params);
  }
}


void handleInstruction(String funcName, JsonObject params) {
  Serial.println();
  Serial.println("Instruction: " + funcName);
  if(funcName == "dw") {
    handleDigitalWriteInstruction(
      params["pin"], 
      params["value"]);
  }
  if(funcName == "d") {
    handleDelayInstruction(
      params["ms"]);
  }
}


void connect_wifi() {
  WiFi.forceSleepWake();
  delay( 1 );

  // Bring up the WiFi connection
  WiFi.mode( WIFI_STA );
  WiFi.begin( ssid, password );

  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String do_get_request(String owner) {
  String url = "/prod/controller?owner=" + owner;
  Serial.println("Getting " + String(host) + url);
  HTTPClient https;
  BearSSL::WiFiClientSecure secureClient;
  secureClient.setInsecure();
  int checkBegin = https.begin(secureClient, host, 443, url, true);
//  Serial.print("[Debug] begin: ");
//  Serial.println(checkBegin);
  if(checkBegin != 1) {
    Serial.println("Check begin " + checkBegin);
    Serial.println(1/0);
  }
  
  int code = https.GET();
//  Serial.print("[Debug] code: ");
//  Serial.println(code);
  if(code < 200 || code >= 300) {
    Serial.println("Response code: " + code);
    Serial.println(1/0);
  }

  String responseString = https.getString();
//  Serial.print("[Debug] responseString: ");
//  Serial.println(responseString);
  
  https.end();
  secureClient.stop();

  return responseString;
}

void writeClient(String& s, WiFiClientSecure& client) {
  Serial.println("[client] " + s);
  client.println(s);
}

DynamicJsonDocument deserialize(String json) {
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println("Error during deserialisation");
    Serial.println(error.c_str());

    Serial.println();
    Serial.println("JSON:");
    Serial.println(json);

    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();

  }

  return doc;
}

void loop() {
}

String to_str(long l) {
  char mystr[40];
  sprintf(mystr, "%u", l);
  return mystr;
}
