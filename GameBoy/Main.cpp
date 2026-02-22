#include <iostream>
#include <vector>
#include <array>
#include "Renderer.h"
#include "Input.h"
#include "GameBoy.h"

int main()
{
	Input joyPad = Input();
	Renderer renderer = Renderer(&joyPad);
	GameBoy GB = GameBoy(&joyPad);
	

	while (renderer.ShouldRun)
	{
		if (!renderer.path.empty()) {
			GB.LoadROM(renderer.path);
			renderer.path = "";
		}
		renderer.Event();
		GB.RunFrame(renderer);
	}

	return 0;
}