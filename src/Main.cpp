#include <iostream>
#include "Renderer.h"
#include "Input.h"
#include "GameBoy.h"

int main()
{

	Input joyPad = Input();
	Renderer renderer(&joyPad);
	GameBoy GB(&joyPad);
	

	while (renderer.ShouldRun)
	{
		if (!renderer.path.empty()) {
			bool success = GB.LoadROM(renderer.path);
			std::cout << "ROM load " << (success ? "succeeded" : "FAILED")
					<< ": \"" << renderer.path << "\"" << std::endl;
			renderer.path = "";
		}
		renderer.Event();
		GB.RunFrame(renderer);
	}

	return 0;
}