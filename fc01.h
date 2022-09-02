//
// rocket flight computer mk1
// (c) 2022 icannotfly
//



#pragma once
#ifndef FC01_H
#define FC01_H



// versioning
#define VERSION "1.0.1"
#define BUILD "22"

// onboard LEDs
#define PIN_GREEN_LED 8
#define PIN_RED_LED 13

// constants
#define SEA_LEVEL_PRESSURE 1013.25	// in mbar
#define MAX_NUMFILE_LINE_LENGTH 20	// max number of digits to read in from numfile



// error codes
#define ERROR_BLINK_PULSE_TIME 225	// how long to blink the red led
#define ERROR_BLINK_PAUSE_TIME 1500 // how long to pause between blink patterns
enum ErrorCode
{
	Error_NoError = 0,
	Error_BaroNotInitialized = 1,
	Error_SdNotInitialized = 2,
	Error_SdCardNotUsable = 3
};



// error handling
void HandleError(ErrorCode e);



#endif
