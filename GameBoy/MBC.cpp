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
        if (RomAddress < rom->size())
            return rom->at(RomAddress);
        return 0xFF;
    }

    if (address >= 0x4000 && address < 0x8000)
    {
        u8 bank = CurrentRomBank % RomBanks;
        if (bank == 0) bank = 1;

        u32 Offset = 0x4000 * bank;
        u32 RomAddress = Offset + (address - 0x4000);

        if (RomAddress < rom->size())
            return rom->at(RomAddress);
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

u8 MBC3::Read(u16 address)
{
    if (address >= 0x0000 && address < 0x4000)
        return (*rom)[address];
    else if (address >= 0x4000 && address < 0x8000)
    {
        u8 bank = CurrentRomBank & 0x7F;
        if (bank == 0) bank = 1;
        u32 offset = (bank * 0x4000) + (address - 0x4000);
        if (offset < rom->size())
            return (*rom)[offset];
    }
    else if (address >= 0xA000 && address < 0xC000)
    {
        if (!RamEnabled)
            return 0xFF;
        if (SelectedRegister <= 0x03)
        {
            u32 offset = (0x2000 * SelectedRegister) + (address - 0xA000);
            if (offset < Ram.size())
                return Ram[offset];
            return 0xFF;
        }
        if (SelectedRegister >= 0x08 && SelectedRegister <= 0x0C && HasRTC)
        {
            if (RTC_Latched)
            {
                switch (SelectedRegister)
                {
                case 0x08: return Lsecond;
                case 0x09: return Lminute;
                case 0x0A: return Lhour;
                case 0x0B: return LDL;
                case 0x0C: return LDH;
                }
            }
            else {
                u8 sec, min, hour, dl, dh;
                ComputeRTC(sec, min, hour, dl, dh);

                switch (SelectedRegister)
                {
                case 0x08: return sec;
                case 0x09: return min;
                case 0x0A: return hour;
                case 0x0B: return dl;
                case 0x0C: return dh;
                }
            }
        }
    }
    
    return 0xFF;
}
void MBC3::Write(u16 address, u8 value)
{
    if (address >= 0x0000 && address < 0x2000)
    {
        RamEnabled = (value & 0x0F) == 0x0A;
    }
    else if (address >= 0x2000 && address < 0x4000)
    {
        CurrentRomBank = value & 0x7F;
        if (CurrentRomBank == 0)
            CurrentRomBank = 1;
    }
    else if (address >= 0x4000 && address < 0x6000)
    {
        if (value <= 0x03)
        {
            CurrentRamBank = value & 0x03;
            SelectedRegister = value & 0x0F;
            isRTC = false;
        }
        if (value >= 0x08 && value <= 0x0C && HasRTC)
        {
            SelectedRegister = value & 0x0F;
            isRTC = true;
        }
    }
    else if (address >= 0x6000 && address < 0x8000)
    {
        if (value == 0x00)
            LatchArmed = true;
        if (value == 0x01 && LatchArmed)
        {
            u8 sec, min, hour, dl, dh;
            ComputeRTC(sec, min, hour, dl, dh);

            Lsecond = sec;
            Lminute = min;
            Lhour = hour;
            LDL = dl;
            LDH = dh;

            RTC_Latched = true;
            LatchArmed = false;
        }
    }
    else if (address >= 0xA000 && address < 0xC000)
    {
        if (!isRTC && RamEnabled && !Ram.empty())
        {
            u16 offset = (0x2000 * CurrentRamBank) + (address - 0xA000);
            if (offset < Ram.size())
                Ram[offset] = value;
        }
        else if (isRTC && RamEnabled && HasRTC)
        {
            u8 s, m, h, dl, dh;
            ComputeRTC(s, m, h, dl, dh);

            switch (SelectedRegister)
            {
            case 0x08: s = value % 60; break;
            case 0x09: m = value % 60; break;
            case 0x0A: h = value % 24; break;
            case 0x0B: dl = value; break;
            case 0x0C: dh = value & 0x01; break;
            }

            u64 days = ((u64)dh << 8) | dl;

            u64 newTotal =
                s +
                m * 60 +
                h * 3600 +
                days * 86400;

            RTC_Offset = newTotal;
            RTC_Base = GetCurrentTime();
        }
    }

}
u64 MBC3::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());

    return seconds.count();
}
void MBC3::ComputeRTC(u8& sec, u8& min, u8& hour, u8& dl, u8& dh)
{
    using namespace std::chrono;

    u64 now = GetCurrentTime();
    u64 total = RTC_Offset + (now - RTC_Base);

    sec = total % 60;
    min = (total / 60) % 60;
    hour = (total / 3600) % 24;

    u64 days = total / 86400;

    dl = days & 0xFF;
    dh = (days >> 8) & 0x01;
}