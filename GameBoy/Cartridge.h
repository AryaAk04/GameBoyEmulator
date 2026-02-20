#pragma once
#include <iostream>
#include <vector>
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
	std::vector<u8> romData;
	Cartridge(const std::string& path);
	void GetInfo(const std::vector<u8> rom);
	MBC_Type GetType(u8 byte);
	u16 GetRomBanks(u8 byte);
	u8 GetRamBanks(u8 byte);
	void MakeCart();
	std::unique_ptr<MBC> mbc;

	u8 Read(u16 address) {
		if(mbc)
			return mbc->Read(address);
	};

	void Write(u16 address, u8 value)
	{
		if (mbc)
			mbc->Write(address, value);
	};

private:
	u8 MBC_Byte;
	MBC_Type Type;

	u8 RomByte;
	u16 RomBanksCount;

	u8 RamByte;
	u8 RamBanksCount;
};

