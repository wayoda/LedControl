#include "Move.hpp"
#include "Reverse.hpp"

//TODO: get native test working

void process(){
    UNITY_BEGIN();
    RUN_TEST(test_reverse_simple);
    RUN_TEST(reverse_Block);
    RUN_TEST(test_left_right);
    RUN_TEST(test_right_left);
    RUN_TEST(test_up_down);
    RUN_TEST(test_down_up);
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