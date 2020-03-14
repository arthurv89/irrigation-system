/* 
 * 16
 * 5
 * 4
 * 0
 * 2
 * 14
 * 12
 * 13
 * 15
 * 3
 * 
 */
 
//const int ledPins[3] = {5, 4, 0};
//const int ledPins[3] = {14, 12, 13};
const int ledPins[] = {D1};

const int on_value = LOW;
const int off_value = HIGH;

const long interval = 0;
const long blink_period = 5000;
const int times = 1;

void setup() {
 Serial.begin(115200);
}

void loop() {
  handle();
  Serial.println();
  delay(interval);
}

void handle() {
  for(int p=0; p<sizeof(ledPins); p++) {
    pinMode(ledPins[p], OUTPUT);
    int pin = p;
    Serial.print(ledPins[pin]);
    for(int i=0; i<times; i++) {
      digitalWrite(ledPins[pin], on_value);
      Serial.print(" On");
      delay(blink_period);
  
      digitalWrite(ledPins[pin], off_value);
      Serial.print(" Off");
      delay(blink_period);
    }
    Serial.println();
    pinMode(ledPins[p], INPUT);
  }
}
