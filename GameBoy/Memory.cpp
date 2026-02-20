#include "Memory.h"
#include <iostream>
#include "Cartridge.h"
Memory::Memory(Cartridge* cart, Input* JoyPad)
{
	this->Cart = cart;
	this->JoyPad = JoyPad;
	tCycles = 0;
}

void Memory::WriteByte(u16 address, u8 value)
{
	if (address < 0x8000)
	{
		Cart->Write(address, value);
	}
	if (address >= 0x8000 && address < 0xA000)
		VRAM[address - 0x8000] = value;
	if (address >= 0xA000 && address < 0xC000)
	{
		Cart->Write(address, value);
	}
	if (address >= 0xC000 && address < 0xE000)
		WRAM[address - 0xC000] = value;
	if (address >= 0xE000 && address < 0xFE00)
		WRAM[address - 0xE000] = value;
	if (address >= 0xFE00 && address < 0xFEA0)
		OAM[address - 0xFE00] = value;
	if ((address >= 0xFF00 && address < 0xFF80) || address == 0xFFFF)
	{
		switch (address)
		{
		case 0xFF00:
			JoyPad->WriteButton(value);
			break;
		case 0xFF01:
			SerialData = value;
			break;
		case 0xFF02:
			SerialControl = value;
			if ((SerialControl & 0x81) == 0x81)
			{
				std::cout << static_cast<char>(SerialData) << std::flush;
				SerialControl &= ~0x80;
			}
			break;
		case 0xFF04:
			timer.DIV = 0;
			break;
		case 0xFF05:
			timer.TIMA = value;
			break;
		case 0xFF06:
			timer.TMA = value;
			break;
		case 0xFF07:
			timer.TAC = value;
			break;
		case 0xFF0F:
			IF = value;
			break;
		case 0xFF40:
			LCDC = value;
			break;
		case 0xFF41:
		{
			u8 current = STAT & 0x03;
			u8 Writable = value & 0x7C;
			STAT = current | Writable;
			break;
		}
		case 0xFF42:
			SCY = value;
			break;
		case 0xFF43:
			SCX = value;
			break;
		case 0xFF44:
			break;
		case 0xFF45:
			LYC = value;
			break;
		case 0xFF46: {
			if (value >= 0x00 && value <= 0xDF)
			{
				u16 SourceBase = value << 8;
				for (int i = 0; i < 160; i++)
				{
					u8 data = ReadByte(SourceBase + i);
					OAM[i] = data;
				}
			}
			break;
		}
		case 0xFF47:
			BGP = value;
			break;
		case 0xFF48:
			OBP0 = value;
			break;
		case 0xFF49:
			OBP1 = value;
			break;
		case 0xFF4A:
			WY = value;
			break;
		case 0xFF4B:
			WX = value;
			break;
		case 0xFF50:
			BootEnable = !(value & 0x01);
			break;
		case 0xFFFF:
			IE = value;
			break;
		default:
			IO[address - 0xFF00] = value;
			break;
		}
	}
	if (address >= 0xFF80 && address < 0xFFFF)
		HRAM[address - 0xFF80] = value;
	
}

u8 Memory::ReadByte(u16 address)
{
	if (BootEnable && address < 0x100)									// Boot Rom
		return BootRom[address];
	if (address < 0x8000 || (address >= 0xA000 && address < 0xC000))	// Cartridge
		return Cart->Read(address);
	if (address >= 0x8000 && address <= 0xA000)							// VRAM
		return VRAM[address - 0x8000];
	if (address >= 0xC000 && address < 0xE000)							// WRAM
		return WRAM[address - 0xC000];
	if (address >= 0xE000 && address < 0xFE00)							// Echo Ram
		return WRAM[address - 0xE000];
	if (address >= 0xFE00 && address < 0xFEA0)							// OAM
		return OAM[address - 0xFE00];
	//if (address >= 0xFEA0 && address < 0xFF00)							// Prohibited Area
		//return;
	if ((address >= 0xFF00 && address < 0xFF80) || address == 0xFFFF)	// IO Registers
	{
		switch (address)
		{
		case 0xFF00:
			return JoyPad->ReadButton();
		case 0xFF01:
			return SerialData;
		case 0xFF02:
			return SerialControl;
		case 0xFF04:
			return timer.DIV;
		case 0xFF05:
			return timer.TIMA;
		case 0xFF06:
			return timer.TMA;
		case 0xFF07:
			return timer.TAC;
		case 0xFF0F:
			return IF;
		case 0xFF40:
			return LCDC;
		case 0xFF41:
			return STAT;
		case 0xFF42:
			return SCY;
		case 0xFF43:
			return SCX;
		case 0xFF44:
			return LY;
		case 0xFF45:
			return LYC;
		case 0xFF46:
			return DMA;
		case 0xFF47:
			return BGP;
		case 0xFF48:
			return OBP0;
		case 0xFF49:
			return OBP1;
		case 0xFF4A:
			return WY;
		case 0xFF4B:
			return WX;
		case 0xFFFF:
			return IE;
		default:
			return IO[address - 0xFF00];
		}
	}
	if (address >= 0xFF80 && address < 0xFFFF)							// HRAM
		return HRAM[address - 0xFF80];

}

void Memory::SetInterruptFlag(u8& Register, u8 flag, bool state)
{
	if (state)
		Register |= flag;
	else
		Register &= ~flag;
}

void Memory::UpdateTimer(u8 Cycle)
{
	divCounter += Cycle;
	tCycles = Cycle;
	if (divCounter >= 256)
	{
		divCounter -= 256;
		timer.DIV++;
	}

	if (timer.TAC & 0x04)
	{
		timaCounter += Cycle;
		u16 IncRate = 0;
		switch (timer.TAC & 0x03)
		{
		case 0x00:
			IncRate = 1024;
			break;
		case 0x01:
			IncRate = 16;
			break;
		case 0x02:
			IncRate = 64;
			break;
		case 0x03:
			IncRate = 256;
			break;
		}
		while (timaCounter >= IncRate)
		{
			timaCounter -= IncRate;
			if (timer.TIMA == 0xFF)
			{
				timer.TIMA = timer.TMA;
				SetInterruptFlag(IF, TIMER_FLAG, true);
			}
			else
				timer.TIMA++;
		}
	}
}