#include <FS.h>   // Include the SPIFFS library

String path = "/wifi.txt";

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  SPIFFS.begin();

  if (SPIFFS.exists(path)) {
    File f = SPIFFS.open(path, "r");
    read_file(f);
    f.close();
  } else {
    File f = SPIFFS.open(path, "w");
    write_file(f);
    f.close();
  }
}

void read_file(File f) {
    Serial.println("Reading file");
    String contents = "";
    while (f.available()){
      contents += f.readStringUntil('\n');
    }
    Serial.println("Read file fully:" + contents);
}

void write_file(File f) {
    Serial.println("Writing file");
    int bytesWritten = f.print("TEST SPIFFS");
    Serial.println(String(bytesWritten) + " bytes written");
}

void loop() {
}
