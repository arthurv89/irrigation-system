#include "IRunner.h"
void setupMidget(IRunner* _iRunner);

void loopMidget();

void do_big_calculation();

void no_button_press();

String create_payload();

void deep_sleep(int interval, long long beforeMillis, long long afterMillis);

void sleep(int us);

void fetch_settings();

void submit_results(String payload);

void update_code();

StaticJsonDocument<200> get_settings();

boolean is_wifi_setup();

void setup_wifi();
