#pragma once
#include <vector>
#include <iostream>
#include <chrono>
#include "Defs.h"


class MBC
{
public:

	MBC(std::vector<u8>* rom);
	virtual ~MBC() = default;

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
	//std::vector<u8>* Rom;
	std::vector<u8> Ram;
	u16 RomBanks;
	u8 RamBanks;

	bool HasBattery;

	u8 CurrentRomBank;
	u8 CurrentRamBank;
	bool RamEnabled;
	bool mode;
public:
	MBC1(std::vector<u8>* rom, u16 romCount, u8 ramCount, bool battery)
		:MBC(rom), RomBanks(romCount), RamBanks(ramCount), HasBattery(battery)
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

class MBC3 : public MBC
{
	std::vector<u8> Ram;
	u16 RomBanks;
	u8 RamBanks;

	u8 CurrentRomBank;
	u8 CurrentRamBank;

	bool RamEnabled;
	
	u8 SelectedRegister;
	bool isRTC = false;

	bool HasRTC;
	bool HasBattery;

	u8 Lsecond;
	u8 Lminute;
	u8 Lhour;
	u8 LDL;
	u8 LDH;

	bool RTC_Latched;
	bool LatchArmed;

	u64 RTC_Base;
	u64 RTC_Offset;

	u64 GetCurrentTime();
	void ComputeRTC(u8& sec, u8& min, u8& hour, u8& dl, u8& dh);

public:
	MBC3(std::vector<u8>* rom, u16 romCount, u8 ramCount, bool hasBattery, bool hasRTC)
		:MBC(rom), RomBanks(romCount), RamBanks(ramCount), HasBattery(hasBattery), HasRTC(hasRTC)
	{
		if (RamBanks > 0)
			Ram.resize(RamBanks * 0x2000);
		if (HasRTC)
		{
			RTC_Base = GetCurrentTime();
			RTC_Offset = 0;
		}

		CurrentRamBank = 0;
		CurrentRomBank = 1;
		RamEnabled = 0;
	}

	void Write(u16 address, u8 value) override;
	u8 Read(u16 address) override;
};