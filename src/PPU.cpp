#include "PPU.h"

PPU::PPU(Memory* Mem)
{
	this->Mem = Mem;
	mode = Mode::OAM_Scan;
}

void PPU::LY_LYC()
{
	if (Mem->LY == Mem->LYC)
	{
		SetFlag(Mem->STAT, LYC_LY_EQ, true);
		if (GetFlag(Mem->STAT, LYC_INT_SELECT))
		{
			Mem->SetInterruptFlag(Mem->IF, LCD_FLAG, true);
		}
	}
	else
		SetFlag(Mem->STAT, LYC_LY_EQ, false);
}

void PPU::SetFlag(u8& reg, u8 flag, bool state)
{
	if (state)
		reg |= flag;
	else
		reg &= ~flag;
}

bool PPU::GetFlag(u8 reg, u8 flag)
{
	return (reg & flag) != 0;
}

void PPU::Set_STAT_Mode()
{
	switch (mode)
	{
	case Mode::H_Blank:
		SetFlag(Mem->STAT, MODE_BIT1, false);
		SetFlag(Mem->STAT, MODE_BIT2, false);
		break;

	case Mode::V_Blank:
		SetFlag(Mem->STAT, MODE_BIT1, true);
		SetFlag(Mem->STAT, MODE_BIT2, false);
		break;

	case Mode::OAM_Scan:
		SetFlag(Mem->STAT, MODE_BIT1, false);
		SetFlag(Mem->STAT, MODE_BIT2, true);
		break;

	case Mode::Drawing:
		SetFlag(Mem->STAT, MODE_BIT1, true);
		SetFlag(Mem->STAT, MODE_BIT2, true);
		break;
	}
}

void PPU::Step(u8 Cycle)
{
	Dot += Cycle;
	switch (mode)
	{
		case Mode::OAM_Scan:
		{ 
			if (Dot >= 80)
			{
				ScanOAM();
				Dot -= 80;
				mode = Mode::Drawing;
				Set_STAT_Mode();
			}
			break;
		}

		case Mode::Drawing:
		{
			if (Dot >= 172)
			{
				DrawLine();
				Dot -= 172;
				mode = Mode::H_Blank;
				Set_STAT_Mode();
				if (GetFlag(Mem->STAT, M0_INT_SELECT))
					SetFlag(Mem->IF, LCD_FLAG, true);
			}
			break;
		}

		case Mode::H_Blank:
		{
			if (Dot >= 204)
			{
				Dot -= 204;
				if (GetFlag(Mem->LCDC, WIN_ENABLE) && Mem->LY >= Mem->WY && Mem->WX <= 166)
					WLY++;
				Mem->LY++;
				LY_LYC();
				if (Mem->LY == 144)
				{
					mode = Mode::V_Blank;
					ShouldDraw = true;
					Set_STAT_Mode();
					SetFlag(Mem->IF, VBLANK_FLAG, true);
					if (GetFlag(Mem->STAT, M1_INT_SELECT))
						SetFlag(Mem->IF, LCD_FLAG, true);
				}
				else
				{
					mode = Mode::OAM_Scan;
					Set_STAT_Mode();
					if (GetFlag(Mem->STAT, M2_INT_SELECT))
						SetFlag(Mem->IF, LCD_FLAG, true);
				}
			}
			break;
		}

		case Mode::V_Blank:
		{
			if (Dot >= 456)
			{
				Dot -= 456;
				Mem->LY++;
				LY_LYC();
				if (Mem->LY >= 154)
				{
					Mem->LY = 0;
					WLY = 0;
					mode = Mode::OAM_Scan;
					Set_STAT_Mode();
					if (GetFlag(Mem->STAT, M2_INT_SELECT))
						SetFlag(Mem->IF, LCD_FLAG, true);
				}
			}
			break;
		}
	}
}

void PPU::ScanOAM()
{
	Sprites.clear();
	u8 SpriteCount = 0;
	u8 Line = Mem->LY;

	for (int i = 0; i < 40; i++)
	{
		u16 address = 0xFE00 + (i * 4);
		u8 SpriteY = Mem->ReadByte(address);
		u8 Height = GetFlag(Mem->LCDC, OBJ_SIZE) ? 16 : 8;
		
		if (Line + 16 >= SpriteY && Line + 16 < SpriteY + Height)
		{
			Sprite s;
			s.Y = Mem->ReadByte(address);
			s.X = Mem->ReadByte(address + 1);
			s.TileID = Mem->ReadByte(address+ 2);
			s.Flags = Mem->ReadByte(address + 3);
			Sprites.push_back(s);
			SpriteCount++;
		}

		if (SpriteCount == 10)
			break;
	}
	std::stable_sort(Sprites.begin(), Sprites.end(), [](const Sprite& a, const Sprite& b) {return a.X < b.X; });
}

void PPU::DrawLine()
{	
	if (!GetFlag(Mem->LCDC, PPU_ENABLE))
		return;
	u16 BGTileMapBase = GetFlag(Mem->LCDC, BG_TILE_MAP) ? 0x9C00 : 0x9800;
	u16 WINTileMapBase = GetFlag(Mem->LCDC, WIN_TILE_MAP) ? 0x9C00 : 0x9800;
	bool isSign = !GetFlag(Mem->LCDC, BG_WIN_TILES);

	u8 SCX = Mem->ReadByte(0xFF43);
	u8 SCY = Mem->ReadByte(0xFF42);
	u8 LY = Mem->LY;
	u8 WX = Mem->WX;
	u8 WY = Mem->WY;

	std::array<Shade, 4> palette = DecodePalette(Mem->BGP);

	u8 TileID = 0;
	u8 Byte1 = 0;
	u8 Byte2 = 0;

	for (int x = 0; x < 160; x++)
	{
		if (GetFlag(Mem->LCDC, WIN_ENABLE) && x >= (WX - 7) && LY >= WY) {

			u8 WindowX = x - (WX - 7);
			u8 WindowY = WLY;

			if (x == (WX - 7) || WindowX % 8 == 0)
			{
				u8 TileX = WindowX / 8;
				u8 TileY = WindowY / 8;

				u8 PixelY = WindowY % 8;

				u16 TileMapAddress = WINTileMapBase + (TileY * 32) + TileX;
				TileID = Mem->ReadByte(TileMapAddress);

				u16 DataAddress = isSign ? (0x9000 + ((i8)TileID * 16)) : (0x8000 + (TileID * 16));

				Byte1 = Mem->ReadByte(DataAddress + (PixelY * 2));
				Byte2 = Mem->ReadByte(DataAddress + (PixelY * 2) + 1);
			}
			u8 PixelX = WindowX % 8;
			u8 Index = 7 - PixelX;

			u8 bit1 = (Byte1 >> Index) & 1;
			u8 bit2 = (Byte2 >> Index) & 1;
			u8 ID = (bit2 << 1) | bit1;

			SetPixel(x, LY, palette[ID]);
		}
		else if (GetFlag(Mem->LCDC, BG_WIN_ENABLE))
		{
			u8 X = x + SCX;
			u8 Y = LY + SCY;

			if (x == 0 || (X % 8 == 0))
			{
				u8 TileX = X / 8;
				u8 TileY = Y / 8;

				u16 TileMapAddress = BGTileMapBase + (TileY * 32) + TileX;
				TileID = Mem->ReadByte(TileMapAddress);

				u8 PixelY = Y % 8;
				u16 DataAddress;
				if (isSign)
					DataAddress = 0x9000 + ((i8)TileID * 16);
				else
					DataAddress = 0x8000 + (TileID * 16);

				Byte1 = Mem->ReadByte(DataAddress + (PixelY * 2));
				Byte2 = Mem->ReadByte(DataAddress + (PixelY * 2) + 1);
			}

			u8 PixelX = X % 8;
			u8 Index = 7 - PixelX;

			u8 bit1 = (Byte1 >> Index) & 1;
			u8 bit2 = (Byte2 >> Index) & 1;
			u8 ID = (bit2 << 1) | bit1;

			SetPixel(x, LY, palette[ID]);
		}
	
	}
	
	if (!GetFlag(Mem->LCDC, OBJ_ENABLE))
		return;

	bool isTall = GetFlag(Mem->LCDC, OBJ_SIZE);
	u8 Height = isTall ? 16 : 8;
	
	if (Sprites.size() == 0)
		return;
	reverse(Sprites.begin(), Sprites.end());

	for (const auto& S : Sprites)
	{
		std::array<Shade, 4> OBJPalette = GetFlag(S.Flags, DMG_PALETTE) ? DecodePalette(Mem->OBP1) : DecodePalette(Mem->OBP0);

		u8 RowInTile = LY + 16 - S.Y;
		if(GetFlag(S.Flags, Y_FLIP))
			RowInTile = (Height - 1) - RowInTile;

		u8 RealID = isTall ? (S.TileID & 0xFE) : S.TileID;
		u16 address = 0x8000 + (RealID * 16) + (RowInTile * 2);

		Byte1 = Mem->ReadByte(address);
		Byte2 = Mem->ReadByte(address + 1);

		for (int bit = 0; bit < 8; bit++)
		{
			int SX = S.X - 8 + bit;
			if (SX < 0 || SX >= 160)
				continue;
			u8 BitToRead;
			if (GetFlag(S.Flags, X_FLIP))
				BitToRead = bit;
			else
				BitToRead = 7 - bit;
			
			u8 bit1 = (Byte1 >> BitToRead) & 1;
			u8 bit2 = (Byte2 >> BitToRead) & 1;
			u8 ID = (bit2 << 1) | bit1;
			if (ID == 0)
				continue;
			
			bool BGPriority = GetFlag(S.Flags, PRIORITY);

			if (Display[(LY * 160) + SX] != palette[0] && BGPriority)
				continue;
			SetPixel(SX, LY, OBJPalette[ID]);
		}
	}
}

std::array<Shade, 4> PPU::DecodePalette(u8 value)
{
	Shade C0 = DecodePaletteColor(value & 0x03);
	Shade C1 = DecodePaletteColor((value >> 2) & 0x03);
	Shade C2 = DecodePaletteColor((value >> 4) & 0x03);
	Shade C3 = DecodePaletteColor((value >> 6) & 0x03);

	std::array<Shade, 4> C = { C0, C1, C2, C3 };
	return C;
}

Shade PPU::DecodePaletteColor(u8 value)
{
	value &= 0x03;
	switch (value)
	{
	case 0:
		return Shade::Lightest;
	case 1:
		return Shade::Light;
	case 2:
		return Shade::Dark;
	case 3:
		return Shade::Darkest;
	}
	return Shade::Lightest;
}

void PPU::SetPixel(u8 X, u8 Y, Shade color)
{
	Display[(Y * 160) + X] = color;
}