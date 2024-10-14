#pragma once

#include "types.h"

bool contains(const string& input, char c)
{
    int idx = input.find_first_of(c);
    return idx != string::npos;
}

bool starts_with(const string& input, char c)
{
    if (input.length() <= 0) return false;
    return input[0] == c;
}

bool case_insensitive_compare(char a, char b)
{
    return tolower(a) == tolower(b);
}

bool contains(const string& input, const string& substring)
{
    return search(input.begin(),
                  input.end(),
                  substring.begin(),
                  substring.end(),
                  case_insensitive_compare) != input.end();
}

bool starts_with(const string& input,
                 const string& startSequence,
                 bool caseSensitive = false)
{
    if (caseSensitive)
    {
        int cmpResult = input.compare(0, startSequence.length(), startSequence);
        return cmpResult == 0;
    }
    else
    {
        return equal(startSequence.begin(),
                     startSequence.end(),
                     input.begin(),
                     case_insensitive_compare);
    }
}

bool ends_with(const string& input,
               const string& substring,
               bool caseSensitive = false)
{
    int inStartIdx = input.length() - substring.length();
    if (inStartIdx < 0) return false;

    int subIdx = 0;
    for (int i = inStartIdx; i < input.length(); i++)
    {
        char cIn = input[i];
        char cSub = substring[subIdx++];

        if (caseSensitive)
        {
            if (!case_insensitive_compare(cIn, cSub)) return false;
        }
        else
        {
            if (cIn != cSub) return false;
        }
    }

    return true;
}

/**
 * Trims the basic whitespace characters: space, \t \n \r
 * Can be extended to trim more with the second parameter
 */
void trim(string& input, string additionalTrimChars = "")
{
    const string trimCharacters = additionalTrimChars.append(" \t\n\r");

    input.erase(0, input.find_first_not_of(trimCharacters));
    input.erase(input.find_last_not_of(trimCharacters) + 1);
}

/**
 * removes the starting sequence from the string if it exists
 */
void trim_start(string& input, const string& sequence)
{
    if (starts_with(input, sequence))
    {
        input.erase(0, sequence.length());
    }
}

void remove_until(string& input, char c, bool last = true)
{
    int idx;
    if (last)
        idx = input.find_last_of(c);
    else
        idx = input.find_first_of(c);

    if (idx != string::npos)
    {
        // we want to erase the last 'c' as well
        input.erase(0, idx + 1);
    }
}

Split split_at(int idx, const string& input)
{
    Split split = {};
    if (idx != string::npos)
    {
        split.head = input.substr(0, idx);
        split.tail = input.substr(idx + 1);
        split.found = true;
    }
    else
    {
        split.head = input;
        split.tail = "";
    }

    return split;
}

Split split_next(const string& input, char sep)
{
    int idx = input.find_first_of(sep);
    return split_at(idx, input);
}

Split split_last(const string& input, char sep)
{
    int idx = input.find_last_of(sep);
    return split_at(idx, input);
}

Split split_last_path(const string& input)
{
    int sep1 = input.find_last_of('/');
    int sep2 = input.find_last_of('\\');

    // unix root dir
    if (sep1 == 0 || sep2 == 0)
    {
        Split rootDir = {"/", input.substr(1, input.length() - 1)};
        return rootDir;
    }

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
 * will exit automatically when string exceeds 4096 characters
 */
vector<string> split_all(const char* input, char sep, bool skipEmpty = true)
{
    vector<string> splits;

    int idx = 0;
    int prevIdx = 0;
    const char* c = input;

    while (idx < PATH_MAX)
    {
        if (*c == sep || *c == '\0')
        {
            int length = idx - prevIdx;

            // TODO: i allocate this here but never deallocate it
            // - so a new one for every string.split call
            char* substr = new char[length + 1];
            for (int i = 0; i < length; i++)
            {
                substr[i] = input[prevIdx + i];
            }
            // without the null terminator, we'll get problems while printing
            substr[length] = '\0';

            // NOTE: since this is a string vector, it will
            // allocate new memory here for the array also
            // using vector<char*> would eleviate the problem,
            // but then we would need to handle null
            // termination ourselfes, so we keep it that way

            if (!skipEmpty || length > 0)
            {
                splits.push_back(substr);
            }
            prevIdx = idx + 1;
        }

        // TODO: pretty ugly -> refactor
        if (*c == '\0') break;

        c++;
        idx++;
    }

    return splits;
}
