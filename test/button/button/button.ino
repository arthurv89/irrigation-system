String version = "1.0";

int pin = D2;

int buttonPressed;

void setup() {
  Serial.begin(115200);
//  digitalWrite(D0, HIGH);

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("Finished Setup");
  Serial.println("");
  Serial.println("");
  Serial.println("");

  buttonPressed = digitalRead(pin);
  Serial.println("buttonPressed? ");
  Serial.println(buttonPressed);
  
  digitalWrite(pin, LOW);
  
  buttonPressed = digitalRead(pin);
  Serial.println("buttonPressed? ");
  Serial.println(buttonPressed);
  
  digitalWrite(pin, HIGH);
  
  buttonPressed = digitalRead(pin);
  Serial.println("buttonPressed? ");
  Serial.println(buttonPressed);
}

void run() {
  x(pin);
//  x(D4);
}

void x(int buttonPin) {
  pinMode(buttonPin, INPUT);

  String str = version + " ";
  
  int buttonPressed = digitalRead(buttonPin);
  str  += "PIN " + String(buttonPin) + " = " + buttonPressed;
  Serial.println(str);

//  if(buttonPressed == 1) {
//    reboot(str);
//  } else {
//    no_button_press(str);
//  }
}

//void reboot(String str) {
//  str  += " IF: Rebooting";
//    Serial.println(str);
//    delay(3000);
////    ESP.deepSleep( 1 * 1000, WAKE_RF_DEFAULT);
//}
//
//void no_button_press(String str) {
//  str += " ELSE";
//    Serial.println(str);
////    ESP.deepSleep( 3000 * 1000, WAKE_RF_DEFAULT);
//  
//}

void loop() {
    run();
    Serial.println("");
    delay(500);
}
