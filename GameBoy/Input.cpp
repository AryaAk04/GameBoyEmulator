#include "Input.h"

Input::Input()
{
    J.A = 0;
    J.B = 0;
    J.Select = 0;
    J.Start = 0;
    J.Up = 0;
    J.Down = 0;
    J.Left = 0;
    J.Right = 0;

    JoyPadRegister = 0xCF;
}

u8 Input::ReadButton()
{
    u8 Selection = JoyPadRegister & 0x30;
    u8 Result = JoyPadRegister & 0xF0;

    Result |= 0x0F;

    if (!(Selection & 0x10))
    {
        if (J.Right)
            Result &= ~0x01;
        if (J.Left)
            Result &= ~0x02;
        if (J.Up)
            Result &= ~0x04;
        if (J.Down)
            Result &= ~0x08;
    }

    if (!(Selection & 0x20))
    {
        if (J.A)
            Result &= ~0x01;
        if (J.B)
            Result &= ~0x02;
        if (J.Select)
            Result &= ~0x04;
        if (J.Start)
            Result &= ~0x08;
    }

    return Result;
}

void Input::WriteButton(u8 value)
{
    u8 Mask = value & 0x30;
    JoyPadRegister = 0xC0 | Mask | 0x0F;
}

void Input::Press(u8 num)
{
    u8 Mask = JoyPadRegister & 0x30;
    switch (num)
    {
    case 1:
        isPressed = J.A;
        J.A = 1;
        if (!isPressed && (Mask == 0x10 || Mask == 0x00))
            TriggerInt();
        break;

    case 2: 
        isPressed = J.B;
        J.B = 1;
        if (isPressed && (Mask == 0x10 || Mask == 0x00))
            TriggerInt();
        break;

    case 3: 
        isPressed = J.Select;
        J.Select = 1; 
        if (isPressed && (Mask == 0x10 || Mask == 0x00))
            TriggerInt();
        break;

    case 4: 
        isPressed = J.Start;
        J.Start = 1; 
        if (isPressed && (Mask == 0x10 || Mask == 0x00))
            TriggerInt();
        break;

    case 5: 
        isPressed = J.Right;
        J.Right = 1; 
        if (isPressed && (Mask == 0x20 || Mask == 0x00))
            TriggerInt();
        break;

    case 6: 
        isPressed = J.Left;
        J.Left = 1; 
        if (isPressed && (Mask == 0x20 || Mask == 0x00))
            TriggerInt();
        break;

    case 7: 
        isPressed = J.Up;
        J.Up = 1; 
        if (isPressed && (Mask == 0x20 || Mask == 0x00))
            TriggerInt();
        break;

    case 8: 
        isPressed = J.Down;
        J.Down = 1; 
        if (isPressed && (Mask == 0x20 || Mask == 0x00))
            TriggerInt();
        break;
    }
}

void Input::Release(u8 num)
{
    
    switch (num)
    {
    case 1: J.A = 0; break;
    case 2: J.B = 0; break;
    case 3: J.Select = 0; break;
    case 4: J.Start = 0; break;
    case 5: J.Right = 0; break;
    case 6: J.Left = 0; break;
    case 7: J.Up = 0; break;
    case 8: J.Down = 0; break;
    }
}

void Input::SetIntCallback(std::function<void()> callback)
{
    ReqInt = callback;
}

void Input::TriggerInt()
{
    if (ReqInt)
        ReqInt();
}