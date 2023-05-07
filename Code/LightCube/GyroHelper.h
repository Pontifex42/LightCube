#pragma once
typedef enum { UP, DOWN, LEFT, RIGHT, FRONT, BACK } orientation_t;

orientation_t GetOrientation();
extern orientation_t currentLay;
extern bool isShaking;

void SetupGyro();
void LoopGyro();
