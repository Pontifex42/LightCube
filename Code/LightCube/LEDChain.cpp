#include "PinConfig.h"
#include <Adafruit_NeoPixel.h>
#include "LEDChain.h"
#include "GyroHelper.h"

//#define DEBUG_LEDCHAIN
#ifndef DEBUG_LEDCHAIN
#ifdef DEBUG_PRINTLN
#undef DEBUG_PRINTLN
#define DEBUG_PRINTLN(a)
#undef DEBUG_PRINT
#define DEBUG_PRINT(a)
#endif
#endif

const uint32_t palette[NUM_PREDEFINED_COLORS] = { COL_BLUE, COL_GREEN, COL_RED, COL_YELLOW, COL_CYAN, COL_PURPLE, COL_WHITE };

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 32 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_LED_BUS, NEO_GRB + NEO_KHZ800);

int currentLEDMode = 0;
#define NUMBER_OF_LED_MODES 5

void TestLED()
{
	for (int i = 0; i < NUMPIXELS; i++)
		pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // RED
	pixels.show();   // Send the updated pixel colors to the hardware.
	delay(400); // Pause before next pass through loop

	for (int i = 0; i < NUMPIXELS; i++)
		pixels.setPixelColor(i, pixels.Color(128, 128, 128)); // RED
	pixels.show();   // Send the updated pixel colors to the hardware.
	delay(400); // Pause before next pass through loop

	for (int i = 0; i < NUMPIXELS; i++)
		pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // RED
	pixels.show();   // Send the updated pixel colors to the hardware.
	delay(400); // Pause before next pass through loop
}

void PrintColorCodes()
{
	char hex[30];
	uint32_t col;
	col = pixels.Color(0, 0, 255); // BLUE
	sprintf(hex, "%x", col);
	DEBUG_PRINTLN("BLUE: 0x" + String(hex));

	col = pixels.Color(255, 255, 0); // YELLOW
	sprintf(hex, "%x", col);
	DEBUG_PRINTLN("YELLOW: 0x" + String(hex));
	col = pixels.Color(0, 255, 255); // CYAN
	sprintf(hex, "%x", col);
	DEBUG_PRINTLN("CYAN: 0x" + String(hex));
	col = pixels.Color(255, 255, 255); // WHITE
	sprintf(hex, "%x", col);
	DEBUG_PRINTLN("WHITE: 0x" + String(hex));
	col = pixels.Color(0, 255, 0); // GREEN
	sprintf(hex, "%x", col);
	DEBUG_PRINTLN("GREEN: 0x" + String(hex));
	col = pixels.Color(255, 0, 0); // RED
	sprintf(hex, "%x", col);
	DEBUG_PRINTLN("RED: 0x" + String(hex));
}

void IncrementLEDMode()
{
	currentLEDMode++;
	if (currentLEDMode >= NUMBER_OF_LED_MODES)
		currentLEDMode = 0;
}

void SetWholeCube(uint8_t r, uint8_t g, uint8_t b, uint8_t bright)
{
	pixels.setBrightness(bright);
	uint32_t col = pixels.Color(r, g, b);
	for (int i = 0; i < NUMPIXELS; i++)
		pixels.setPixelColor(i, col);
	pixels.show();   // Send the updated pixel colors to the hardware.
}

// Mode 1: color whole cube in a single color, depending on lay
void Mode1()
{
	static ulong lastActTime = 0;
	ulong now = millis();
	if ((now - lastActTime) < 200) // handle only once every 200 ms
		return;
	lastActTime = now;

	uint32_t col = palette[currentLay];
	for (int i = 0; i < NUMPIXELS; i++)
		pixels.setPixelColor(i, col);
	pixels.setBrightness(DEFAULT_BRIGHTNESS);
	pixels.show();   // Send the updated pixel colors to the hardware.
}

#ifdef IGNORE // did not turn out as a good effect.
// Mode X: Give 6 colors to 6 sides and make it independent of orientation
void ModeX()
{
	// Colors for UP/DOWN/LEFT/RIGHT/FRONT/BACK
	const uint32_t sideCol[6] = { COL_BLUE, COL_RED, COL_YELLOW, COL_CYAN, COL_GREEN, COL_PURPLE };
	uint32_t orient_colors[6];

	// LED ordering is:
	// DOWN, 6xFRONT, UP, UP, 6xLEFT, DOWN, DOWN, 6x BACK, UP, UP, 6xRIGHT, DOWN
	const orientation_t LEDS[NUMPIXELS] = { DOWN, DOWN, FRONT, FRONT, FRONT, FRONT, UP, UP,
											UP, UP, LEFT, LEFT, LEFT, LEFT, DOWN, DOWN,
											DOWN, DOWN, BACK, BACK, BACK, BACK, UP, UP,
											UP, UP, RIGHT, RIGHT, RIGHT, RIGHT, DOWN, DOWN };
	switch (currentLay)
	{
	case LEFT: // right is up, 
		orient_colors[DOWN] = sideCol[RIGHT];
		orient_colors[UP] = sideCol[LEFT];
		orient_colors[LEFT] = sideCol[DOWN];
		orient_colors[RIGHT] = sideCol[UP];
		orient_colors[FRONT] = sideCol[FRONT];
		orient_colors[BACK] = sideCol[BACK];
		break;
	case RIGHT:
		orient_colors[DOWN] = sideCol[LEFT];
		orient_colors[UP] = sideCol[RIGHT];
		orient_colors[LEFT] = sideCol[UP];
		orient_colors[RIGHT] = sideCol[DOWN];
		orient_colors[FRONT] = sideCol[FRONT];
		orient_colors[BACK] = sideCol[BACK];
		break;
	case FRONT:
		orient_colors[DOWN] = sideCol[BACK];
		orient_colors[UP] = sideCol[FRONT];
		orient_colors[LEFT] = sideCol[LEFT];
		orient_colors[RIGHT] = sideCol[RIGHT];
		orient_colors[FRONT] = sideCol[DOWN];
		orient_colors[BACK] = sideCol[UP];
		break;
	case BACK:
		orient_colors[DOWN] = sideCol[FRONT];
		orient_colors[UP] = sideCol[BACK];
		orient_colors[LEFT] = sideCol[LEFT];
		orient_colors[RIGHT] = sideCol[RIGHT];
		orient_colors[FRONT] = sideCol[UP];
		orient_colors[BACK] = sideCol[DOWN];
		break;
	case UP: // up/down flipped
		orient_colors[DOWN] = sideCol[UP];
		orient_colors[UP] = sideCol[DOWN];
		orient_colors[LEFT] = sideCol[LEFT];
		orient_colors[RIGHT] = sideCol[RIGHT];
		orient_colors[FRONT] = sideCol[FRONT];
		orient_colors[BACK] = sideCol[BACK];
		break;
	case DOWN: // default: all sides have standard colors
	default:
		orient_colors[DOWN] = sideCol[DOWN];
		orient_colors[UP] = sideCol[UP];
		orient_colors[LEFT] = sideCol[LEFT];
		orient_colors[RIGHT] = sideCol[RIGHT];
		orient_colors[FRONT] = sideCol[FRONT];
		orient_colors[BACK] = sideCol[BACK];
		break;
	}

	for (int i = 0; i < NUMPIXELS; ++i)
	{
		orientation_t dir = LEDS[i];
		uint32_t col = orient_colors[dir];
		pixels.setPixelColor(i, col);
	}
	pixels.setBrightness(DEFAULT_BRIGHTNESS);
	pixels.show();
}
#endif

// Mode 2: Merry-go-round of colors, ignore gyro
void Mode2()
{
	static ulong lastWaveTime = 0;
	ulong now = millis();
	if ((now - lastWaveTime) < 25)
		return;
	lastWaveTime = now;

	pixels.setBrightness(255); // brightness considered by own calculation

	// sequence:
	// first side starts glowing up to the max
	// if reached max, it is dimmed and in parrallel next side starts glowing in this color
	// if a side reaches 0, it switches to next color
	static int coloridx[4] = { 0, -1, -1, -1 };
	static uint32_t intensity[4] = { 0, 0, 0, 0 };
	static uint32_t increment[4] = { 5, 5, 5, 5 };

	for (int side = 0; side < 4; ++side)
	{
		uint32_t col = (coloridx > 0) ? palette[coloridx[side]] : 0x00000000u; // dark

		int intens = pixels.gamma8(intensity[side]);
		uint8_t r = (uint8_t)(col >> 16), g = (uint8_t)(col >> 8), b = (uint8_t)col;
		r = (r * intens) >> 8;
		g = (g * intens) >> 8;
		b = (b * intens) >> 8;
		int startLED = side * 8;
		for(int i = startLED; i < (startLED + 8); ++i)
			pixels.setPixelColor(i, r, g, b);
	}
	pixels.show();

	for (int side = 0; side < 4; ++side)
	{
		if (coloridx[side] < 0)
			continue;

		intensity[side] += increment[side];

		if (intensity[side] >= 255)
		{
			increment[side] = -increment[side];

			// switch next side to this color. Next sides brightness is 0 at this moment.
			if (side != 3)
				coloridx[side + 1] = coloridx[side];
		}
		else if (intensity[side] <= 0)
		{
			increment[side] = -increment[side];

			// switch this side to next color
			if (side == 0)
			{
				coloridx[0]++;
				if (coloridx[side] >= (NUM_PREDEFINED_COLORS - 1))
					coloridx[side] = 0;
			}
		}
	}
}

// Mode 3: Police emergency light
void Mode3()
{
	static ulong lastToggleTime = 0;
	ulong now = millis();
	if ((now - lastToggleTime) < 600)
		return;
	lastToggleTime = now;

	static bool toggle = false;
	uint32_t colA = toggle ? COL_BLUE : COL_RED;
	uint32_t colB = toggle ? COL_RED : COL_BLUE;
	toggle = !toggle;

	// start with blue at front/back leds and red at left/right LEDs
	// than toogle every 600 ms
	if ((currentLay == DOWN) || (currentLay == UP))
	{
		for (int i = 0; i < 8; ++i)
			pixels.setPixelColor(i, colA);
		for (int i = 8; i < 16; ++i)
			pixels.setPixelColor(i, colB);
		for (int i = 16; i < 24; ++i)
			pixels.setPixelColor(i, colA);
		for (int i = 24; i < 32; ++i)
			pixels.setPixelColor(i, colB);
	}
	else
	{
			for (int i = 0; i < 4; ++i)
				pixels.setPixelColor(i, colA);
			for (int i = 4; i < 12; ++i)
				pixels.setPixelColor(i, colB);
			for (int i = 12; i < 20; ++i)
				pixels.setPixelColor(i, colA);
			for (int i = 20; i < 28; ++i)
				pixels.setPixelColor(i, colB);
			for (int i = 28; i < NUMPIXELS; ++i)
				pixels.setPixelColor(i, colA);
	}
	pixels.setBrightness(DEFAULT_BRIGHTNESS);
	pixels.show();
}

// Mode 4: sinus waving all colors
void Mode4()
{
	static ulong lastWaveTime = 0;
	ulong now = millis();
	if ((now - lastWaveTime) < 25)
		return;
	lastWaveTime = now;

	static int intensityIdx = 0;
	static int increment = 5;
	static int colorIdx = 0;

	int col = palette[colorIdx];
	int intens = pixels.gamma8(intensityIdx);
	pixels.setBrightness(intens);
	for (int i = 0; i < NUMPIXELS; i++)
		pixels.setPixelColor(i, col);
	pixels.show();   // Send the updated pixel colors to the hardware.

	intensityIdx += increment;
	if (intensityIdx > 250)
	{
		intensityIdx = 245;
		increment = -increment;
	}
	else if (intensityIdx < 0)
	{
		intensityIdx = 5;
		increment = -increment;
		colorIdx++;
		if (colorIdx >= NUM_PREDEFINED_COLORS)
			colorIdx = 0;
	}
}

// Mode 5: Give 2 colors to sides depending on lay
void Mode5()
{
	static uint32_t col1 = COL_BLUE;
	static uint32_t col2 = COL_RED;
	static orientation_t lastLay = FRONT;

	static bool firstrun = true;
	if ((!firstrun) && (lastLay == currentLay))
			return;
	else
		firstrun = false;
	
	lastLay = currentLay;
	
	col1 = palette[random(NUM_PREDEFINED_COLORS - 1)];
	do
	{
		col2 = palette[random(NUM_PREDEFINED_COLORS - 1)];
	} while (col1 == col2);

	for (int i = 0; i < NUMPIXELS/2; ++i)
		pixels.setPixelColor(i, col1);

	for (int i = NUMPIXELS / 2; i < NUMPIXELS; ++i)
		pixels.setPixelColor(i, col2);
	
	pixels.setBrightness(DEFAULT_BRIGHTNESS);
	pixels.show();
}

// For shaking mode, do flicker in all colors like colors are mixed by the shaking
void MakeRandomColors()
{
	static ulong lastRandom = 0;
	ulong now = millis();
	if ((now - lastRandom) < 300)
		return;
	lastRandom = now;

	uint32_t col;
	for (int segment = 0; segment < 8; segment++)
	{
		long rnd = random(NUM_PREDEFINED_COLORS-1);
		switch (rnd)
		{
		case 0:
			col = COL_BLUE;
			break;
		case 1:
			col = COL_GREEN;
			break;
		case 2:
			col = COL_RED;
			break;
		case 3:
			col = COL_YELLOW;
			break;
		case 4:
			col = COL_CYAN;
			break;
		case 5:
			col = COL_PURPLE;
			break;
		default:
			continue; // pixel remains unchanged
			break;
		}

		int start = segment * 4;
		for(int i = start; i < (start + 4); i++)
			pixels.setPixelColor(i, col);
	}
	pixels.setBrightness(DEFAULT_BRIGHTNESS / 2);
	pixels.show();
}

void SetupLEDChain()
{
	pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
	pixels.clear();
	pixels.setBrightness(DEFAULT_BRIGHTNESS);
	pixels.show();

	randomSeed(42);
	// PrintColorCodes();
}

void LoopLEDChain()
{
	if (isShaking)
	{
		MakeRandomColors();
		return;
	}

	// TestLED();
	switch (currentLEDMode)
	{
	case 0:
		Mode1();
		break;
	case 1:
		Mode2();
		break;
	case 2:
		Mode3();
		break;
	case 3:
		Mode4();
		break;
	case 4:
		Mode5();
		break;
	default:
		Mode1();
		break;
	}
}