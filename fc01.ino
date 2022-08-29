//
// rocket flight computer mk1
// (c) 2022 icannotfly
// GPLv3
//



#include <Arduino.h>
#include <Wire.h>
#include "fc01.h"
#include <Adafruit_DPS310.h>    // barometer
#include <SdFat.h>              // sd card



//// globals
// barometer
Adafruit_DPS310 baro;       // barometer itself

// sd card
SdFat sd;
#define CHIP_SELECT 4
SdFile numfile;             // contains the sequential id number of the last file used

// logging
SdFile logfile;
char logfilePath[13] = "";	// max filename length (8) + . + ext + null terminator = 8+1+3+1 = 13



void setup() {
    Serial.println("Starting up...");



	//// lights
	pinMode(PIN_RED_LED, OUTPUT);
	pinMode(PIN_GREEN_LED, OUTPUT);
	//// /lights



    //// barometer
    // initialize
    if (!baro.begin_I2C(DPS310_I2CADDR_DEFAULT))
    {
        Serial.println("FATAL: unable to initialize DPS310 on 0x77");
		HandleError(Error_BaroNotInitialized);
    }
    // configure
    baro.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
    baro.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
    //// /barometer



    //// sd card
    // initialize card
    if (!sd.begin(CHIP_SELECT, SPI_FULL_SPEED/* gotta go fast */))
    {
        //sd.initErrorHalt();
		HandleError(Error_SdNotInitialized);
    }

	char numfilePath[] = "lastnum";
	Serial.print("Checking for numfile... ");
	if (!numfile.open(numfilePath, O_CREAT | O_RDWR))	// must be RDWR to create a file, READ won't work
	{
		//sd.errorHalt("unable to open or create numfile");
		HandleError(Error_SdCardNotUsable);
	}



	// set up the logfile
	// first, determine what we should number this logfile by looking inside numfile
	char line[MAX_NUMFILE_LINE_LENGTH]; // line that was read in
	long nextLogfileNum = 0;	// next number in sequence

	// we're only expecting one line, so just read the first one
	numfile.fgets(line, sizeof(line), "\n");

	// done reading, close the file
	numfile.close();

	// was there anything in the file?
	if (strlen(line) > 0)
	{
		// yes
		// chop up line via a token (\n)
		char* token = strtok(line, "\n");

		// then convert that token to a long
		nextLogfileNum = atol(line);
	}
	else
	{
		Serial.print("no numfile exists, will create... ");
		//num is already set to 0 so we eill just write that later
	}

	Serial.print("done!\n");

	// now put it all together
	nextLogfileNum++;
	sprintf(logfilePath, "%08ld.csv", nextLogfileNum);
	Serial.print("Next logfile will be "); Serial.print(logfilePath); Serial.print("\n");

	// write new number to numfile
	if (!numfile.open(numfilePath, O_TRUNC | O_WRITE)) {	// opening as truc is the secret sauce
		//sd.errorHalt("unable to trunc open numfile");
		HandleError(Error_SdCardNotUsable);
	}
	numfile.println(nextLogfileNum);
	numfile.close();

	// create logfile and write header to it
	if (!logfile.open(logfilePath, O_CREAT | O_RDWR | O_AT_END))
	{
		//sd.errorHalt("unable to open logfile");
		HandleError(Error_SdCardNotUsable);
	}
	logfile.print("time (ms),");
	logfile.print("temperature (C),");
	logfile.print("pressure (pa),");
	logfile.print("altitude (m),");
	logfile.print("\n");
	logfile.close();

	// adios
	Serial.print("\n");
    //// /sd card



    Serial.println("Done. Ready!");
}



void loop() {

    Serial.print(millis());
    Serial.print("\t");

    if (baro.pressureAvailable())
    {
		digitalWrite(PIN_GREEN_LED, HIGH);

        sensors_event_t temperatureEvent;
        sensors_event_t pressureEvent;
        baro.getEvents(&temperatureEvent, &pressureEvent);
		float altitude = (pow((SEA_LEVEL_PRESSURE / pressureEvent.pressure), 0.190223) - 1.0) * (temperatureEvent.temperature + 273.15) / 0.0065;

        Serial.print(temperatureEvent.temperature);
        Serial.print("\t");
        Serial.print(pressureEvent.pressure);
        Serial.print("\t");
        Serial.print(altitude);
        Serial.print('\n');



		// write to logfile
		if (!logfile.open(logfilePath, O_WRITE | O_AT_END | O_APPEND))
		{
			//sd.errorHalt("unable to open logfile for logging");
			HandleError(Error_SdCardNotUsable); // this is Very Bad during flight, and we should have a different way of handling this later on (write to a ram buffer, then dump if card gets plugged back in? //TODO)
		}

		#define COMMA logfile.print(",")
		logfile.print(millis()); COMMA;
		logfile.print(temperatureEvent.temperature); COMMA;
		logfile.print(pressureEvent.pressure); COMMA;
		logfile.print(altitude); COMMA;
		logfile.print("\n");

		logfile.close();

		digitalWrite(PIN_GREEN_LED, LOW);
    }



    delay(250);
}



void HandleError(ErrorCode e)
{
	if (e == Error_NoError)
	{
		return;
	}

	while (1)
	{
		for (int i = 0; i < int(e); i++)
		{
			digitalWrite(PIN_RED_LED, HIGH);
			delay(ERROR_BLINK_PULSE_TIME);
			digitalWrite(PIN_RED_LED, LOW);
			delay(ERROR_BLINK_PULSE_TIME);
		}
		delay(ERROR_BLINK_PAUSE_TIME);
	}
}
