// TODO: refactor this is not good, that the order is kind of messed up here
// already
#pragma once

#include "types.h"

bool contains(string input, char c)
{
    int idx = input.find_first_of(c);
    return idx != string::npos;
}

bool starts_with(string input, char c)
{
    if (input.length() <= 0) return false;
    return input[0] == c;
}

bool starts_with(string input, string startSequence)
{
    int cmpResult = input.compare(0, startSequence.length(), startSequence);
    return cmpResult == 0;
}

Split split_at(int idx, string input)
{
    Split split = {};
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

Split split_next(string input, char sep)
{
    int idx = input.find_first_of(sep);
    return split_at(idx, input);
}

Split split_last(string input, char sep)
{
    int idx = input.find_last_of(sep);
    return split_at(idx, input);
}

Split split_last_path(string input)
{
    int sep1 = input.find_last_of('/');
    int sep2 = input.find_last_of('\\');

    if (sep1 > sep2)
        return split_at(sep1, input);
    else
        return split_at(sep2, input);
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
