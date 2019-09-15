#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include <sstream>

// Loop variables
long long previousMillis = 0;
const int interval = 300000; // Should be 900000 (15 minutes)

// Channels
const int moisture_sensor_in = A0;


// WIFI
#ifndef STASSID
#define STASSID "NOS-172E"
#define STAPSK  "TVZY3KNX"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;


// AWS endpoint
const char* host = "gktyevilwf.execute-api.us-east-1.amazonaws.com";
const String url = "/prod/post-data";
const int httpsPort = 443;
const char fingerprint[] PROGMEM = "E6 8D 15 A0 C3 FD 67 F7 B2 DF 69 93 6C 80 A8 50 0C 85 EE 9A";

const String owner = "casabatata";
const boolean resetDeviceId = false;
const char prefix[] = "IRSYS-";
const int deviceIdLength = 20;


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

  perform_action(deviceId);
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



void setup_moisture_sensor() {
  pinMode(moisture_sensor_in, INPUT);
}





void perform_action(String deviceId) {
  Serial.println();
  Serial.println("==========");
  setup_moisture_sensor();
  int moisture_value = get_moisture_value();

  connect_wifi();
  do_post_request(moisture_value, owner, deviceId);
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

void send_request(WiFiClientSecure& client, int moisture_value, String owner, String deviceId) {
  client.setTimeout(8000);
  Serial.print("Sending post request to ");
  Serial.println(host);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    Serial.println("Restarting WIFI");
    connect_wifi();
  }

  Serial.print("requesting URL: ");
  Serial.println(url);

  String postData = create_json(moisture_value, owner, deviceId);
  Serial.print("Post data: " );
  Serial.println(postData);
  
  client.println("POST /prod/post-data HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("Cache-Control: no-cache");
  client.println("Content-Type: multipart/form-data");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);
}

String create_json(int moisture_value, String owner, String deviceId) {
//  std::ostringstream stream;
//  stream << "aaaaaaaaaaaaaaaaaaaaa" << "aaaaaaaaaaaaaaaaaaaaa";
//  std::string buf(stream.str());

  char buffer [100];
  sprintf (buffer, "{"
           "\"moisture-value\": %d,"
           "\"owner\": \"%s\","
           "\"deviceId\": \"%s\""
           "}",
         moisture_value,
         owner.c_str(),
         deviceId.c_str());

  return buffer;
}

void do_post_request(int moisture_value, String owner, String deviceId) {
  WiFiClientSecure client;
  send_request(client, moisture_value, owner, deviceId);
  Serial.println("request sent");

  // Handle response
  parse_headers(client);
  int response_length = get_response_length(client);
  String json = read_response(client, response_length);
  DynamicJsonDocument doc = deserialize(json);

  Serial.print("Success: ");
  int success = doc["success"];
  Serial.println(success);
}

String read_response(WiFiClientSecure& client, int response_length) {
  String json = "";
  for (int i = 0; i < response_length - 1; i++) {
    char c = client.read();
    json += c;
  }
  return json;
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

int get_response_length(WiFiClientSecure& client) {
  String response_len_str = client.readStringUntil('|');
  //  Serial.print("response_len_str: ");
  //  Serial.println(response_len_str);

  int response_length = std::atoi(response_len_str.c_str());
  //  Serial.print("response_length: ");
  //  Serial.println(response_length);

  return response_length;
}

void parse_headers(WiFiClientSecure& client) {
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.print("Headers: ");
    Serial.println(line);
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
}

int get_moisture_value() {
  int moisture_value = analogRead(moisture_sensor_in);
  Serial.print("Moisture value: ");
  Serial.println(moisture_value);
  return moisture_value;
}


void loop() {
}

String to_str(long l) {
  char mystr[40];
  sprintf(mystr, "%u", l);
  return mystr;
}
