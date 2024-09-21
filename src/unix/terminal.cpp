#include "../types.h"
#include "types.h"

const int FD_STDIN = 0;
const int FD_STDOUT = 1;
const int FD_STDERR = 2;

char unix_getch()
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



