#include "IRunner.h"
#include <ArduinoJson.h>

void handle(IRunner* iRunner);
void handleInstruction(JsonArray instruction);
void setPin(int pin, int value);
StaticJsonDocument<2048> get_response();
void execute_instructions(StaticJsonDocument<2048> response);
void setLcdText(String text);
