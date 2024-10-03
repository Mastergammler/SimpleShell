#pragma once

#include "colors.h"
#include "commands.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <map>
#include <stack>
#include <stdio.h>
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
using std::search;
using std::stack;
using std::tolower;
using std::vector;

static bool running = true;
static vector<string> binPaths;

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

struct CompletionCache
{
    bool refresh;
    vector<string> completions;
    int index;
    string prev_completion;
};

struct SessionState
{
    CompletionCache path_completions;
    CompletionCache branch_completions;

    // transient for current session
    vector<string> history;
    int history_index;
};

const char CH_ESC = 27;
const char CH_TAB = 9;
const char CH_DEL = 127;
const char ESCS_ARROW_UP = 'A';
const char ESCS_ARROW_DOWN = 'B';
const char ESCS_ARROW_LEFT = 'C';
const char ESCS_ARROW_RIGHT = 'D';
const char ESCS_SHIFT_TAB = 'Z';
const char ESCS_DEL_1 = '3';
const char ESCS_DEL_2 = '~';
