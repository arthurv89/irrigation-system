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
const int ledPins[1] = {LED_BUILTIN};

const int on_value = LOW;
const int off_value = HIGH;

const long interval = 5000;

int i = 0;

void setup() {
 Serial.begin(115200);
 pinMode(ledPins[0], OUTPUT);
    
}

void loop() {
  handle();
  Serial.println();
  delay(interval);
}

void handle() {
  blink_times(0, 3);
}

void blink_times(int pin, int times) {
  Serial.print(ledPins[pin]);
  for(int i=0; i<times; i++) {
    digitalWrite(ledPins[pin], on_value);
    Serial.println("On");
    delay(200);

    digitalWrite(ledPins[pin], off_value);
    Serial.println("Off");
    delay(200);
  }
}
