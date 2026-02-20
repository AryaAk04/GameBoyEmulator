#pragma once
#include "Defs.h"
#include "Color.h"
#include "Input.h"
#include <SDL3/SDL.h>
#include <array>
#include <iostream>
#include <string>
#include <sstream>

class Renderer
{
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

