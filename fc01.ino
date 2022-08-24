//
// rocket flight computer mk1
// (c) 2022 icannotfly
// GPLv3
//



#include <Arduino.h>
#include <Wire.h>

#include "fc01.h"



void setup() {

}



void loop() {
    digitalWrite(PIN_RED_LED, HIGH);
    delay(1000);
    digitalWrite(PIN_RED_LED, LOW);
    delay(1000);
}
