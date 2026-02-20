#include "MBC.h"

MBC::MBC(std::vector<u8>* rom)
{
	this->rom = rom;
}

u8 MBC::Read(u16 address)
{
	return rom->at(address);
}
void MBC::Write(u16 address, u8 value)
{
	return;
}

u8 MBC0::Read(u16 address)
{
	return rom->at(address);
}
void MBC0::Write(u16 address, u8 value)
{
	return;
}

u8 MBC1::Read(u16 address)
{
    if (address < 0x4000)
    {
        // Bank 0 or high bank bits if mode=0
        u8 bank0 = (mode == 0) ? ((CurrentRomBank & 0x60) << 9) : 0;
        u32 RomAddress = bank0 + address;
        if (RomAddress < Rom->size())
            return Rom->at(RomAddress);
        return 0xFF;
    }

    if (address >= 0x4000 && address < 0x8000)
    {
        u8 bank = CurrentRomBank % RomBanks;
        if (bank == 0) bank = 1;

        u32 Offset = 0x4000 * bank;
        u32 RomAddress = Offset + (address - 0x4000);

        if (RomAddress < Rom->size())
            return Rom->at(RomAddress);
        return 0xFF;
    }

    if (address >= 0xA000 && address < 0xC000)
    {
        if (!RamEnabled || Ram.empty()) return 0xFF;

        u8 bank = (mode == 1) ? CurrentRamBank : 0;
        u32 Offset = 0x2000 * bank + (address - 0xA000);
        if (Offset < Ram.size())
            return Ram.at(Offset);
        return 0xFF;
    }

    return 0xFF;
}

void MBC1::Write(u16 address, u8 value)
{
    if (address >= 0x0000 && address < 0x2000)
    {
        RamEnabled = (value & 0x0F) == 0x0A;
    }
    else if (address >= 0x2000 && address < 0x4000)
    {
        // Lower 5 bits of ROM bank number
        u8 lower = value & 0x1F;
        if (lower == 0) lower = 1;
        CurrentRomBank = (CurrentRomBank & 0xE0) | lower;
    }
    else if (address >= 0x4000 && address < 0x6000)
    {
        // Upper bits for ROM or RAM bank depending on mode
        u8 high = value & 0x03;
        if (mode == 0)
            CurrentRomBank = (CurrentRomBank & 0x1F) | (high << 5);
        else
            CurrentRamBank = high;
    }
    else if (address >= 0x6000 && address < 0x8000)
    {
        mode = value & 0x01;
    }
    else if (address >= 0xA000 && address < 0xC000)
    {
        if (!RamEnabled || Ram.empty()) return;

        u8 bank = (mode == 1) ? CurrentRamBank : 0;
        u32 Offset = 0x2000 * bank + (address - 0xA000);
        if (Offset < Ram.size())
            Ram[Offset] = value;
    }
}