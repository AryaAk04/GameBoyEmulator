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


int main()
{
	Cartridge Cart = Cartridge(/*Mario*/);
	Input joyPad = Input();

	Memory Mem = Memory(&Cart, &joyPad);
	CPU cpu = CPU(&Mem);
	u8 Cycles;

	Renderer renderer = Renderer(&joyPad);
	PPU ppu = PPU(&Mem);

	joyPad.SetIntCallback([&Mem]() {
		Mem.SetInterruptFlag(Mem.IF, JOYPAD_FLAG, true);
		}
	);

	cpu.NoBootInit();

	while (renderer.ShouldRun)
	{
		Cycles = cpu.Step();
		ppu.Step(Cycles);
		if (ppu.ShouldDraw) {
			renderer.Step(ppu.Display);
			ppu.ShouldDraw = false;
		}
	}

	return 0;
}