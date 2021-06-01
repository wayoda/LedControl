#include "Move.hpp"
#include "transformation.hpp"
#include "config.hpp"
#include "display.hpp"

void process() {
    UNITY_BEGIN();
    run_display_tests();
    run_config_tests();
    run_transformation_tests();
    run_move_tests();
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
    delay(2000);
    process();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    ArduinoFakeReset();
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delay)).AlwaysReturn();
    When(Method(ArduinoFake(), shiftOut)).AlwaysReturn();
    process();
    return 0;
}

#endif