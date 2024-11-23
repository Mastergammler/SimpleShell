#include "../types.h"

#include <conio.h>
#include <windows.h>

char get_ch()
{
    return _getch();
}

bool shift_is_down()
{
    // requires user32.dll
    return (GetAsyncKeyState(VK_SHIFT) & 0x8000);
}

ActionInput read_action(char c)
{
    if (c == 0 || c == (char)224)
    {
        char next = get_ch();
        switch (next)
        {
            case 72: return ARROW_UP;
            case 80: return ARROW_DOWN;
            case 75: return ARROW_LEFT;
            case 77: return ARROW_RIGHT;
            case 83: return DEL;
        }
    }
    else if (c == CH_DEL || c == CH_BACK)
        return BACKSPACE;
    else if (c == CH_TAB)
    {
        // TEST: does this work running on windows?
        if (shift_is_down())
        {
            return SHIFT_TAB;
        }
        else
            return TAB;
    }

    return NONE;
}
