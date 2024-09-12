#include "types.h"

Split split_next(string input, char sep)
{
    Split split = {};
    int idx = input.find_first_of(sep);
    if (idx != string::npos)
    {
        split.head = input.substr(0, idx);
        split.tail = input.substr(idx + 1);
    }
    else
    {
        split.head = input;
        split.tail = "";
    }

    return split;
}
