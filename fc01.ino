//
// rocket flight computer mk1
// (c) 2022 icannotfly
// GPLv3
//



#include <Arduino.h>
#include <Wire.h>
#include "fc01.h"
#include <Adafruit_DPS310.h>    // barometer




// globals
Adafruit_DPS310 baro;     // barometer



void setup() {
    Serial.println("Starting up...");


    //    barometer
    // initialize
    if (!baro.begin_I2C(DPS310_I2CADDR_DEFAULT))
    {
        Serial.println("FATAL: unable to initialize DPS310 on 0x77");
        digitalWrite(PIN_RED_LED, HIGH);
        while (1) {}; //die
    }
    // configure
    baro.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
    baro.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
    //    barometer



    Serial.println("Done. Ready!");
}



void loop() {

    Serial.print(millis());
    Serial.print("\t");

    if (baro.pressureAvailable())
    {
        digitalWrite(PIN_RED_LED, HIGH);

        sensors_event_t temperatureEvent;
        sensors_event_t pressureEvent;
        baro.getEvents(&temperatureEvent, &pressureEvent);

        Serial.print(temperatureEvent.temperature);
        Serial.print("\t");
        Serial.print(pressureEvent.pressure);
        Serial.print("\t");
#define SEA_LEVEL_PRESSURE 1013.25
        Serial.print((pow((SEA_LEVEL_PRESSURE / pressureEvent.pressure), 0.190223) - 1.0)* (temperatureEvent.temperature + 273.15) / 0.0065);
        Serial.print('\n');

        digitalWrite(PIN_RED_LED, LOW);
    }



    delay(500);
}
