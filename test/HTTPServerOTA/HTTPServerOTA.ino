/**
 * This test application looks every time at a executable binary that is available online.
 * If the file shows an update, the ESP8266 updates itself.
 * This way, we don't have to go to the garden in order to update the code but we can update it from inside.
 *
 * In this project we show this proof of concept by making a call to the local Mac webserver.
 * There are 2 binaries there: the fast blinking binary and the slow blinking binary.
 * We can easily switch by pointing the softlink from one to the other.
 * The Arduino then automatically downloads it and executes that code, and thus changes the blinking pace.
 */

#include <ArduinoOTA.h>
#include "ESP8266httpUpdate.h"

const char* ssid = "CasaBatata"; //your WiFi Name
const char* password = "nopassword";  //Your Wifi Password

bool fast = false;

int speed = -1;
String speed_str = "";

void setup() {
}

void loop() {
  Serial.begin(115200);

  if(fast) {
    speed = 100;
    speed_str = "fast";
  } else {
    speed = 1000;
    speed_str = "slow";
  }
  
  Serial.println();
  Serial.println("Start loop");
  Serial.println("Speed = " + String(speed));

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("");

  Serial.println("Connecting...");
  connect();

  Serial.println("Updating code...");
  update();

  Serial.println("Blinking...");
  blink();
  blink();
  blink();
  blink();
  blink();

  Serial.println("Restart");

  ESP.restart();
}

void blink() {
  Serial.println("On for " + String(speed) + " seconds");
  digitalWrite(LED_BUILTIN, LOW);
  delay(speed);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(speed);
}

void connect() {
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(100);
    Serial.print(++i); Serial.print(' ');
  }
}

void update() {
  ESPhttpUpdate.rebootOnUpdate(false);
  int update_res = ESPhttpUpdate.update("192.168.1.3", 8123, "/bin/blink?speed=" + speed_str);
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
