#pragma once
#include "Defs.h"
#include "Color.h"
#include "Input.h"
#include <SDL3/SDL.h>
#include <array>
#include <iostream>
#include <string>
#include <sstream>

struct Palette {
	u32 colors[4];
	const char* name;
};

const Palette PALETTES[] = {
{{0xFFFFFFFF, 0xFF7B7B7B, 0xFF4A4A4A, 0xFF000000}, "Grayscale"},
{{0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000}, "Original GameBoy"},
{{0xFFE0F8D0, 0xFF88C070, 0xFF346856, 0xFF081820}, "Classic GameBoy"},
{{0xFFFFFFBF, 0xFFBECB7F, 0xFF7F9F7F, 0xFF3F4B3F}, "Pocket"},
{{0xFFFFC0C0, 0xFFC08080, 0xFF804040, 0xFF402020}, "Sepia"},
{{0xFFFFFF00, 0xFFAAAA00, 0xFF555500, 0xFF000000}, "Yellow Scale"},
{{0xFF000000, 0xFF555555, 0xFFAAAAAA, 0xFFFFFFFF}, "Inverted"}
};


class Renderer
{
	int CurrentPaletteIndex = 0;

	void NextPalette() {
		CurrentPaletteIndex = (CurrentPaletteIndex + 1) %
			(sizeof(PALETTES) / sizeof(PALETTES[0]));
	}

	void PreviousPalette() {
		CurrentPaletteIndex = (CurrentPaletteIndex - 1 +
			(sizeof(PALETTES) / sizeof(PALETTES[0]))) %
			(sizeof(PALETTES) / sizeof(PALETTES[0]));
	}

private:
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;

	Colors C = Colors::GrayScale;
	u32 FrameBuffer[GB_WIDTH * GB_HEIGHT];
	
	Input* joypad;

	u8 Decode(int code);

	int frameCount = 0;
	u32 last = SDL_GetTicks();
	float fps = 0.0f;
	void ShowFPS();

	u64 lastCounter = 0;
	double Target = 1.0 / 59.73;
	void LimitFPS();

	void Event();

public:
	Renderer(Input* JoyPad);
	bool ShouldRun;
	void Step(const Shade* shade);
};

