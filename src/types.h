#pragma once

#include "commands.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

const int PATH_MAX = 4096;

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::equal;
using std::getenv;
using std::getline;
using std::pair;
using std::stack;
using std::tolower;
using std::vector;

static bool running = true;
static vector<char*> binPaths;

struct Split
{
    string head;
    string tail;
    bool found;
};

struct PathSplit
{
    string path;
    string search_element;
    bool trailing_path_slash;
};

struct Completion
{
    string text;
    bool dir_changed;
    bool found;
};

struct SessionState
{
    // temporary for current input
    bool refresh_completions;
    vector<string> completions;
    int completion_index;
    string previous_completion;

    // transient for current session
    vector<string> history;
    int history_index;
};
