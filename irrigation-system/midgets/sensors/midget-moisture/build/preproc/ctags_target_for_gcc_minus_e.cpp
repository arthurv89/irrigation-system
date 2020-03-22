# 1 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino"
# 1 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino"
using namespace std;

# 4 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino" 2
# 5 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino" 2
# 6 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino" 2
# 7 "/Users/arthur/workspace/arduino/irrigation-system/midgets/controller-moisture/controller-moisture.ino" 2

class Runner: public IRunner
{
public:
  String getType() {
    return "moisture";
  }

  int getButtonPin() {
    return D2;
  }

  String getWifiName(String deviceId) {
    return "IRSYS-M-" + deviceId;
  }

  void add_sensor_values(JsonObject &doc) {
    doc["moisture"] = get_moisture_value();
  }
};


void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);
}


void loop() {
    loopMidget();
}
