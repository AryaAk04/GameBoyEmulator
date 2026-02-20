#pragma once
#include "Defs.h"
#include "Color.h"
#include "Input.h"
#include <SDL3/SDL.h>
#include <array>
#include <iostream>


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


public:
	Renderer(Input* JoyPad);

	bool ShouldRun;
	void Event();
	void Step(const Shade* shade);
};

