# 1 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-temperature/controller-temperature.ino"
# 1 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-temperature/controller-temperature.ino"
using namespace std;

# 4 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-temperature/controller-temperature.ino" 2
# 5 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-temperature/controller-temperature.ino" 2
# 6 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-temperature/controller-temperature.ino" 2
# 7 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-temperature/controller-temperature.ino" 2
# 8 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-temperature/controller-temperature.ino" 2
# 9 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-temperature/controller-temperature.ino" 2
# 10 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-temperature/controller-temperature.ino" 2




DHT dht(5, 11 /**< DHT TYPE 11 */);

int light_sensor_pin = A0; // select the input pin for the potentiometer

class Runner: public IRunner
{
public:
  String getType() {
    return "temperature";
  }

  int getButtonPin() {
    return D2;
  }

  String getWifiName(String deviceId) {
    return "IRSYS-T-" + deviceId;
  }

  void add_sensor_values(JsonObject &doc) {
    doc["temperature"] = get_temperature_value(dht);
    doc["light"] = analogRead(light_sensor_pin);
  }
};


void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);
  dht.begin();
}


void loop() {
    loopMidget();
}
