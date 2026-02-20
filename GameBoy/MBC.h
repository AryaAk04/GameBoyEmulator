#pragma once
#include <vector>
#include <iostream>
#include "Defs.h"


class MBC
{
public:

	MBC(std::vector<u8>* rom);
	virtual ~MBC() = default;
	//MBC(std::vector<u8> rom, u16 RomBanks, u8 RamBanks);

	std::vector<u8>* rom;

	virtual void Write(u16 address, u8 value);
	virtual u8 Read(u16 address);
};

class MBC0 : public MBC
{
public:
	using MBC::MBC;
	void Write(u16 address, u8 value) override;
	u8 Read(u16 address) override;
};

class MBC1 : public MBC
{
private:
	std::vector<u8>* Rom;
	std::vector<u8> Ram;
	u16 RomBanks;
	u8 RamBanks;

	u8 CurrentRomBank;
	u8 CurrentRamBank;
	bool RamEnabled;
	bool mode;
public:
	MBC1(std::vector<u8>* rom, u16 romCount, u8 ramCount)
		:MBC(rom), Rom(rom), RomBanks(romCount), RamBanks(ramCount)
	{
		if (RamBanks > 0)
			Ram.resize(RamBanks * 0x2000);
		CurrentRamBank = 0;
		CurrentRomBank = 1;
		RamEnabled = false;
	}
	void Write(u16 address, u8 value) override;
	u8 Read(u16 address) override;
};