#pragma once
#include <iostream>
#include "Defs.h"
#include "Memory.h"
#include "Color.h"
#include <array>
#include <vector>
#include <algorithm>

enum class Mode {
	H_Blank,
	V_Blank,
	OAM_Scan,
	Drawing
};

struct Sprite {
	u8 Y;
	u8 X;
	u8 TileID;
	u8 Flags;
};

class PPU
{
private:
	Memory* Mem;
	u16 Dot;
	Mode mode;

	void LY_LYC();
	void SetFlag(u8& reg ,u8 flag, bool state);
	bool GetFlag(u8 reg, u8 flag);
	void Set_STAT_Mode();

	std::array<Shade, 4> DecodePalette(u8 value);
	Shade DecodePaletteColor(u8 value);

	void SetPixel(u8 X, u8 Y, Shade color);

	void DrawLine();
	u8 WLY = 0;

	void ScanOAM();
	std::vector<Sprite> Sprites;

public:
	PPU(Memory* Mem);
	void Step(u8 Cycle);
	bool ShouldDraw = false;
	Shade Display[GB_WIDTH * GB_HEIGHT];
	

};

