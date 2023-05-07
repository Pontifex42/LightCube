#include "PinConfig.h"
#include "GyroHelper.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "LEDChain.h"

//#define DEBUG_GYRO
#ifndef DEBUG_GYRO
#ifdef DEBUG_PRINTLN
#undef DEBUG_PRINTLN
#define DEBUG_PRINTLN(a)
#undef DEBUG_PRINT
#define DEBUG_PRINT(a)
#endif
#endif

#ifdef DEBUG_GYRO
const String orientationText[6] = { "UP", "DOWN", "LEFT", "RIGHT", "FRONT", "BACK" };
#endif


Adafruit_MPU6050 mpu;
orientation_t currentLay; // Says which direction faces down
bool isShaking;
int shakeCounter;


orientation_t GetOrientation()
{
    orientation_t facingdown = DOWN;

    sensors_event_t a, g, temp;
    if (!mpu.getEvent(&a, &g, &temp))
        return facingdown;

    float xacc = (a.acceleration.x);
    float yacc = (a.acceleration.y);
    float zacc = (a.acceleration.z);

    // find maximum, this must be the gravity
    float xabs = abs(xacc);
    float yabs = abs(yacc);
    float zabs = abs(zacc);

    if ((xabs > yabs) && (xabs > zabs))
        facingdown = (xacc > 0) ? FRONT : BACK;
    else if ((yabs > xabs) && (yabs > zabs))
        facingdown = (yacc > 0) ? LEFT : RIGHT;
    else //  if ((zabs > xabs) && (zabs > yabs))
        facingdown = (zacc > 0) ? UP : DOWN;

    DEBUG_PRINTLN(orientationText[facingdown]);

    return facingdown;
}

void SetupGyro()
{
    if (!mpu.begin())
    {
        DEBUG_PRINTLN("Failed to find MPU6050 gyro sensor.");
        while (1)
            ;
    }
    else
        DEBUG_PRINTLN("Found MPU6050 gyro sensor.");

    currentLay = DOWN;
    isShaking = false;
    shakeCounter = 0;
}

void LoopGyro()
{
    static ulong lastChangeTime = 0;
    static ulong lastReadTime = 0;

    ulong now = millis();
    if((now - lastReadTime) < 25)
        return;
    lastReadTime = now;

    // note if cube is shaken
    orientation_t newLay = GetOrientation();
    if (newLay == currentLay)
    {   // nothing has changed. See if it stands still for more than 800 ms, than stop shaking
        if ((now - lastChangeTime) > 500)
        {
            if(isShaking) // Stopping the shaking causes switchingto next LED pattern
                IncrementLEDMode();

            shakeCounter = 0;
            isShaking = false;
        }
        return;
    }

    // Here, something has changed
    currentLay = newLay;
    lastChangeTime = now;

    DEBUG_PRINTLN("new:" + String(newLay) + " cur:" + String(currentLay) + " cnt:" + String(shakeCounter) + " isShaking:" + String(isShaking));

    shakeCounter++;
    if (shakeCounter >= 10)
    {
        shakeCounter = 10;
        isShaking = true;
    }
}
