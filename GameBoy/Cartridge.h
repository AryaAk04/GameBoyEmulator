#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Defs.h"
#include "MBC.h"

enum class MBC_Type
{
	MBC0,
	MBC1,
	MBC2,
	MBC3,
	MBC5,
	UNKNOWN
};

class Cartridge
{
public:
	
	Cartridge(const std::string& path);

	u8 Read(u16 address) {
		if (mbc)
			return mbc->Read(address);
	};

	void Write(u16 address, u8 value)
	{
		if (mbc)
			mbc->Write(address, value);
	};

	bool IsValid() const { return loadedSuccessfully; }

private:

	void GetInfo(const std::vector<u8> rom);
	MBC_Type GetType(u8 byte);
	std::string GetTitle(const std::vector<u8> rom);

	std::vector<u8> romData;

	u16 GetRomBanks(u8 byte);
	u8 GetRamBanks(u8 byte);
	void MakeCart();

	std::unique_ptr<MBC> mbc;

	u8 MBC_Byte;
	MBC_Type Type;

	u8 RomByte;
	u16 RomBanksCount;

	u8 RamByte;
	u8 RamBanksCount;

	bool battery;
	bool RTC;

	bool loadedSuccessfully = false;

	std::string title = "";
};

