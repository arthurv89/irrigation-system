const int ledPin =  4;// the number of the LED pin
int ledState = 0;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)

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
