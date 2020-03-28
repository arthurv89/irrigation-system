int relay_out = 7;

void setup() {
  pinMode(relay_out, OUTPUT);
}

void loop() {
  _digitalWrite(relay_out, HIGH);
  delay(5000);
  _digitalWrite(relay_out, LOW);
  delay(10000);
  
}
