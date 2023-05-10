#pragma once

#define COL_BLUE	0x000000ffu
#define COL_GREEN	0x0000ff00u
#define COL_RED		0x00ff0000u
#define COL_CYAN	0x0000ffb0u		// GREEN | BLUE
#define COL_YELLOW  0x00f0f000u		// RED | GREEN
#define COL_PURPLE  0x00ff0090u		// RED | BLUE
#define COL_WHITE	0x00f0f0f0u		// BLUE | GREEN | RED

#define NUM_PREDEFINED_COLORS 7

#define COL_DARK    0x00000000u		// not really useful

extern const uint32_t palette[NUM_PREDEFINED_COLORS];

#define DEFAULT_BRIGHTNESS 127

void IncrementLEDMode();
void SetWholeCube(uint8_t r, uint8_t g, uint8_t b, uint8_t bright);
void SetupLEDChain();
void LoopLEDChain();
