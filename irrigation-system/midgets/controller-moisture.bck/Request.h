#include "Arduino.h"

#include <WiFiClientSecure.h>

String do_get_request(String url);
String do_post_request(String url, String payload);

void writeClient(String& s, WiFiClientSecure& client);



String getStaticResponse(int x);
