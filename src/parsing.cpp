// TODO: refactor this is not good, that the order is kind of messed up here
// already
#pragma once

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

// not sure if this is correct for both windows and unix
// but should be sufficient
// -> this is already there from dirent.h
// const int PATH_MAX = 4096;

/**
 * Expects a '\0' terminated string,
 * will exit automatically when string exceeds 256 characters
 */
vector<char*> split_all(const char* input, char sep)
{
    vector<char*> splits;

    int idx = 0;
    int lastIdx = 0;
    const char* current = input;

    while (idx < PATH_MAX)
    {
        if (*current == sep || *current == '\0')
        {
            int length = idx - lastIdx;

            char* substr = new char[length];
            for (int i = 0; i < length; i++)
            {
                substr[i] = input[lastIdx + i];
            }
            // NOTE: since this is a string vector, it will allocate new memory
            // here for the array also
            // To prevent this use a vector<char*>
            splits.push_back(substr);
            lastIdx = idx + 1;
        }

        // TODO: pretty ugly -> refactor
        if (*current == '\0') break;

        current++;
        idx++;
    }

    return splits;
}
