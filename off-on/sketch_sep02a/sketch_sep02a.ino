int relay_out = 7;

void setup() {
  pinMode(relay_out, OUTPUT);
}

void loop() {
  digitalWrite(relay_out, HIGH);
  delay(5000);
  digitalWrite(relay_out, LOW);
  delay(10000);
  
}
