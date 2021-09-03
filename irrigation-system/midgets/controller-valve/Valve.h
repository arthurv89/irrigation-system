#include "IRunner.h"
#include <ArduinoJson.h>

void handle(IRunner* iRunner);
void handleInstruction(int valve, int pin, int period_ms);
void setPin(int pin, int value);
StaticJsonDocument<200> get_instructions();
void execute_instructions(StaticJsonDocument<200> instructions, IRunner* iRunner);
void setLcdText(IRunner* iRunner, String text);
