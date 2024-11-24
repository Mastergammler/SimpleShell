#include "../types.h"

#include <conio.h>
#include <consoleapi.h>
#include <windows.h>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    // we're intercepting the Ctrl + C event and telling windows,
    // that we have handled it already
    // This prevents a Ctrl + C press to exit the shell,
    // especially for cases where we call Ctrl + C on a sub process
    if (fdwCtrlType == CTRL_C_EVENT) return TRUE;
    return FALSE;
}

void Init_Console()
{
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
}

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
    // 224 is a signal for extended key codes
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
    // -1 can also be a signal for extended key codes
    // usually 3 should handle ctrl + c, but on unix i'm getting -1
    else if (c == -1 || c == 3)
    {
        return CTRL_C;
    }

    return NONE;
}
