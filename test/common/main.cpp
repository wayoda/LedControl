#include "Move.hpp"
#include "Reverse.hpp"

void process(){
    UNITY_BEGIN();
    RUN_TEST(test_reverse_simple);
    RUN_TEST(reverse_Block);

    run_move_tests();
    run_move_multi_tests();
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