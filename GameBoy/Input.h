#pragma once
#include "Defs.h"
#include <iostream>
#include <array>
#include <functional>

// In GameBoy, 0 Means The Button Is Being Pressed and 1 Means The Button Is Being Released!
class Input
{
	struct JP {
		bool A;
		bool B;
		bool Select;
		bool Start;
		bool Up;
		bool Down;
		bool Left;
		bool Right;
	}J;

private:
	u8 JoyPadRegister;
	std::function<void()> ReqInt;
	void TriggerInt();
	bool isPressed = false;

public:


	void SetIntCallback(std::function<void()> callback);


	void WriteButton(u8 value);
	u8 ReadButton();
	Input();
	void Press(u8 number);
	void Release(u8 number);
};

