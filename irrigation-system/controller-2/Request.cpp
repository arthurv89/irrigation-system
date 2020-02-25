#include "Request.h"
#include "Utils.h"

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#include <HttpClient.h>
#include <EthernetClient.h>


// AWS endpoint
//const String host = "http://192.168.1.3";
//const int port = 8000;
//const String owner = "casabatata";


String do_get_request(String url) {
  WiFiClient client;

  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  String response = "";
  if (http.begin(client, url)) {  // HTTP


    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        response = http.getString();
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  return response;
}


String do_post_request(String url, String payload) {
  HTTPClient httpClient;
  httpClient.begin(url);
  httpClient.addHeader("Content-Type","application/x-www-form-urlencoded");

  int httpCode = httpClient.POST(payload);

  String response = "";
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
    response = httpClient.getString();
  }

//  String content = "{\"temperature\":\"14\"}";
//  client.println("Content-Length: 16"); //insert, well, your content length
//  client.println(content);
  

  Serial.println("httpcode");
  Serial.println(httpCode);
  httpClient.end();

  return response;
}

void writeClient(String& s, WiFiClientSecure& client) {
  Serial.println("[client] " + s);
  client.println(s);
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
