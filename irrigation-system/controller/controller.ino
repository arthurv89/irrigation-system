using namespace std;

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <sstream>
#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

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
const char* host = "irrigation-system.s3.amazonaws.com";

struct RelayMapping {
   String id;
   int pinNo;
};

struct RelayMapping createRelayMapping(String id, int pinNo) {
   struct RelayMapping device;

   device.id = id;
   device.pinNo = pinNo;

   return device;
}


RelayMapping moistureSensorDeviceIds[] = {
  createRelayMapping("IRSYS-20223537419405", D1),
  createRelayMapping("IRSYS-18856796498573", D2)
};
//map<String, int> deviceIds;
//deviceIds.push_back("IRSYS-20223537419405", D1);
//deviceIds.push_back("IRSYS-18856796498573", D2);

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





void perform_action() {
  Serial.println();
  Serial.println("==========");
  
  connect_wifi();
  
  for ( int i = 0; i < NUMITEMS(moistureSensorDeviceIds); i++ ) {
    Serial.println(i);
    struct RelayMapping relayMapping = moistureSensorDeviceIds[i];
    Serial.println(relayMapping.id);
    String response = do_get_request(owner, relayMapping.id);
    Serial.println(response);
    DynamicJsonDocument jsonObj = deserialize(response);
    handle(jsonObj, relayMapping.pinNo);
  }
}

void handle(DynamicJsonDocument& jsonObj, int digitalPin) {
  int moisture_value = jsonObj["moisture_value"];
  Serial.print("moisture_value ");
  Serial.println(moisture_value);
  
  boolean shouldWater = moisture_value > wateringThreshold;
  Serial.print("shouldWater ");
  Serial.println(shouldWater);
  
  if(shouldWater) {
    Serial.print("Turning on pin ");
    Serial.println(digitalPin);
    pinMode(digitalPin, OUTPUT);
    digitalWrite(digitalPin, HIGH);
    delay(10000);
    Serial.print("Turning off pin ");
    Serial.println(digitalPin);
    digitalWrite(digitalPin, LOW);
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

String do_get_request(String owner, String deviceId) {
  String url = "/" + owner + "/" + deviceId;
  Serial.println("Getting " + String(host) + url);
  HTTPClient https;
  BearSSL::WiFiClientSecure secureClient;
  secureClient.setInsecure();
  int checkBegin = https.begin(secureClient, host, 443, url, true);
  if(checkBegin != 1) {
    Serial.println("Check begin " + checkBegin);
    Serial.println(1/0);
  }
  
  int code = https.GET();
  if(code < 200 || code >= 300) {
    Serial.println("Response code: " + code);
    Serial.println(1/0);
  }

  String responseString = https.getString();
  
  https.end();
  secureClient.stop();

  return responseString;
}

void writeClient(String& s, WiFiClientSecure& client) {
  Serial.println("[client] " + s);
  client.println(s);
}

DynamicJsonDocument deserialize(String& json) {
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
