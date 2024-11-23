#include "../types.h"
#include "types.h"

const int FD_STDIN = 0;
const int FD_STDOUT = 1;
const int FD_STDERR = 2;

char get_ch()
{
    char buf = 0;
    struct termios old = {0};

    if (tcgetattr(FD_STDIN, &old) < 0) perror("tcsetattr()");

    // putting terminal in raw mode and disable immediate display
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;

    // waiting for at least on character input
    old.c_cc[VMIN] = 1;
    // waiting indefinently (until 1 char is received)
    old.c_cc[VTIME] = 0;

    if (tcsetattr(FD_STDIN, TCSANOW, &old) < 0) perror("tcsetattr ICANON");

    if (read(0, &buf, 1) < 0) perror("read()");

    // reset original mode and apply settings, after all input operations
    // completed
    // TODO: do i need to do this after every single call??
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;

    if (tcsetattr(FD_STDIN, TCSADRAIN, &old) < 0) perror("tcsettattr ~ICANON");

    return buf;
}

ActionInput read_action(char c)
{
    if (c == ESCS_START)
    {
        // termios returns a secape sequence instead of single characters
        char seq[3];
        seq[0] = get_ch();
        seq[1] = get_ch();
        seq[2] = '\0';
        if (seq[0] == '[')
        {
            switch (seq[1])
            {
                case ESCS_ARROW_UP: return ARROW_UP;
                case ESCS_ARROW_DOWN: return ARROW_DOWN;
                case ESCS_ARROW_LEFT: return ARROW_LEFT;
                case ESCS_ARROW_RIGHT: return ARROW_RIGHT;
                // DEL has sequence '3~'
                case ESCS_DEL_1:
                {
                    char next = get_ch();
                    if (next == ESCS_DEL_2)
                    {
                        return DEL;
                    }
                }
                break;
                case ESCS_SHIFT_TAB: return SHIFT_TAB;
            }
        }
    }
    else if (c == CH_DEL || c == CH_BACK)
        return BACKSPACE;
    else if (c == CH_TAB)
        return TAB;

    return NONE;
}
