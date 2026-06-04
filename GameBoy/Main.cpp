#include <iostream>
#include "Renderer.h"
#include "Input.h"
#include "GameBoy.h"
#include <Windows.h>

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	Input joyPad = Input();
	Renderer renderer(&joyPad);
	GameBoy GB(&joyPad);
	

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