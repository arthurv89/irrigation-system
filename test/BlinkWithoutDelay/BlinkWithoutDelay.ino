const int ledPin =  4;
int ledState = 0;
unsigned long previousMillis = 0;
const long interval = 4000;

void setup() {
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledState == 0) {
      ledState = 1;
      pinMode(ledPin, OUTPUT);
      Serial.println("ON");
    } else {
      ledState = 0;
      pinMode(ledPin, OUTPUT);
      Serial.println("OFF");
    }

    digitalWrite(ledPin, ledState);
  }
  delay(1000);
}
