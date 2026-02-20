#pragma once
#include <cstdint>
#include <iostream>

#define GB_WIDTH 160
#define GB_HEIGHT 144
#define SCALE 6

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


// CPU Flags
constexpr u8 Z_FLAG = 1 << 7;
constexpr u8 N_FLAG = 1 << 6;
constexpr u8 H_FLAG = 1 << 5;
constexpr u8 C_FLAG = 1 << 4;

// IF & IE Flags
constexpr u8 JOYPAD_FLAG = 1 << 4;
constexpr u8 SERIAL_FLAG = 1 << 3;
constexpr u8 TIMER_FLAG = 1 << 2;
constexpr u8 LCD_FLAG = 1 << 1;
constexpr u8 VBLANK_FLAG = 1 << 0;

// LCDC flags
constexpr u8 PPU_ENABLE = 1 << 7;
constexpr u8 WIN_TILE_MAP = 1 << 6;
constexpr u8 WIN_ENABLE = 1 << 5;
constexpr u8 BG_WIN_TILES = 1 << 4;
constexpr u8 BG_TILE_MAP = 1 << 3;
constexpr u8 OBJ_SIZE = 1 << 2;
constexpr u8 OBJ_ENABLE = 1 << 1;
constexpr u8 BG_WIN_ENABLE = 1 << 0;

// STAT Flags
constexpr u8 LYC_INT_SELECT = 1 << 6;
constexpr u8 M2_INT_SELECT = 1 << 5;
constexpr u8 M1_INT_SELECT = 1 << 4;
constexpr u8 M0_INT_SELECT = 1 << 3;
constexpr u8 LYC_LY_EQ = 1 << 2;
constexpr u8 MODE_BIT2 = 1 << 1;
constexpr u8 MODE_BIT1 = 1 << 0;

// Sprite Byte 3 Flags
constexpr u8 PRIORITY = 1 << 7;
constexpr u8 Y_FLIP = 1 << 6;
constexpr u8 X_FLIP = 1 << 5;
constexpr u8 DMG_PALETTE = 1 << 4;
