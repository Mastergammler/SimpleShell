#pragma once

#include "commands.h"
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
using std::getenv;
using std::getline;
using std::pair;
using std::stack;
using std::vector;

static bool running = true;
static vector<char*> binPaths;

struct Split
{
    string head;
    string tail;
};

struct PathSplit
{
    string path;
    string file_name;
};
