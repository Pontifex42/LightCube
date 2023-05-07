#include <Arduino.h>
#include "LEDChain.h"
#include "GyroHelper.h"
#include "Remote.h"

// #define DEBUG_INO
#ifndef DEBUG_INO
#ifdef DEBUG_PRINTLN
#undef DEBUG_PRINTLN
#define DEBUG_PRINTLN(a)
#undef DEBUG_PRINT
#define DEBUG_PRINT(a)
#endif
#endif


void setup(void)
{
	Serial.begin(115200);
	SetupLEDChain(); // switch off random glowing lights fast
	SetupRemote();
	SetupGyro();
}

void loop()
{
	LoopRemote();
	if (IsRemoteConnected())
		return;

	LoopGyro();
	LoopLEDChain();
}
