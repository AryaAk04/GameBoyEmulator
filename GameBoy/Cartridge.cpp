#include "Cartridge.h"
#include <fstream>
#include <iostream>

Cartridge::Cartridge(const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open ROM: " << path << std::endl;
        return;
    }

    size_t size = file.tellg(); 
    file.seekg(0, std::ios::beg);        

    romData.resize(size);
    if (!file.read(reinterpret_cast<char*>(romData.data()), size)) {
        std::cerr << "Failed to read ROM data.\n";
        return;
    }
    GetInfo(romData);
    file.close();
    MakeCart();
    std::cout << "Loaded ROM successfully (" << size << " bytes)" << std::endl;
    return;
}

void Cartridge::GetInfo(const std::vector<u8> rom)
{
    MBC_Byte = rom[0x0147];
    Type = GetType(MBC_Byte);

    RomByte = rom[0x0148];
    RomBanksCount = GetRomBanks(RomByte);

    RamByte = rom[0x0149];
    RamBanksCount = GetRamBanks(RamByte);
}

MBC_Type Cartridge::GetType(u8 byte)
{
    switch (byte)
    {
    case 0x00:
    case 0x08:
    case 0x09:
        return MBC_Type::MBC0;

    case 0x01:
    case 0x02:
    case 0x03:
        return MBC_Type::MBC1;

    case 0x05:
    case 0x06:
        return MBC_Type::MBC2;

    case 0x0F:
        battery = true;
        RTC = true;
        return MBC_Type::MBC3;
    case 0x10:
        battery = true;
        RTC = true;
        return MBC_Type::MBC3;
    case 0x11:
        return MBC_Type::MBC3;
    case 0x12:
        return MBC_Type::MBC3;
    case 0x13:
        battery = true;
        RTC = false;
        return MBC_Type::MBC3;

    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
        return MBC_Type::MBC5;

    default:
        return MBC_Type::UNKNOWN;
    }
}

u16 Cartridge::GetRomBanks(u8 byte)
{
    switch (byte)
    {
    case 0x00:
        return 2;
    case 0x01:
        return 4;
    case 0x02:
        return 8;
    case 0x03:
        return 16;
    case 0x04:
        return 32;
    case 0x05:
        return 64;
    case 0x06:
        return 128;
    case 0x07:
        return 256;
    case 0x08:
        return 512;
    }
}

u8 Cartridge::GetRamBanks(u8 byte)
{
    switch (byte)
    {
    case 0x00:
        return 0;
    case 0x02:
        return 1;
    case 0x03:
        return 4;
    case 0x04:
        return 16;
    case 0x05:
        return 8;
    }
}

void Cartridge::MakeCart()
{
    switch (Type)
    {
    case MBC_Type::MBC0:
        mbc = std::make_unique<MBC0>(&romData);
        break;
    case MBC_Type::MBC1:
        mbc = std::make_unique<MBC1>(&romData, RomBanksCount, RamBanksCount);
        break;
    case MBC_Type::MBC3:
        mbc = std::make_unique<MBC3>(&romData, RomBanksCount, RamBanksCount, battery, RTC);
        break;
    default:
        mbc = std::make_unique<MBC0>(&romData);
        break;
    }
}