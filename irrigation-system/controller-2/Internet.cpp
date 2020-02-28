#include "Request.h"
#include "Properties.h"

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

void connect_wifi(String wifiName, String password) {
  WiFi.forceSleepWake();
  delay( 1 );

  // Bring up the WiFi connection
  WiFi.mode( WIFI_STA );
  WiFi.begin( wifiName, password );

  Serial.println();
  Serial.println("connecting to " + wifiName);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
