#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "Cartridge.h"
#include "CPU.h"
#include "Memory.h"
#include "Renderer.h"
#include "Color.h"
#include "PPU.h"
#include "Input.h"


class GameBoy
{
public:
    std::unique_ptr<Cartridge> cart;
    std::unique_ptr<Memory> mem;
    std::unique_ptr<CPU> cpu;
    std::unique_ptr<PPU> ppu;
    Input* joypad;

    GameBoy(Input* input) : joypad(input) {}

    bool LoadROM(const std::string& path) {

        auto newCart = std::make_unique<Cartridge>(path);


        if (!newCart->IsValid()) return false;

        cart = std::move(newCart);
        mem = std::make_unique<Memory>(cart.get(), joypad);
        cpu = std::make_unique<CPU>(mem.get());
        ppu = std::make_unique<PPU>(mem.get());

        return true;
    }

    void RunFrame(Renderer& renderer) {
        if (!cpu) return;

        u8 cycles = cpu->Step();
    
        ppu->Step(cycles);
        if (ppu->ShouldDraw) {
            renderer.Step(ppu->Display);
            ppu->ShouldDraw = false;
        }
    }
};

