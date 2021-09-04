#include "IRunner.h"
#include <ArduinoJson.h>

void handle(IRunner* iRunner);
void handleInstruction(JsonArray instruction);
void setPin(int pin, int value);
StaticJsonDocument<200> get_response();
void execute_instructions(StaticJsonDocument<200> instructions);
void setLcdText(String text);
