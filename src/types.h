#pragma once

#include "commands.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getenv;
using std::getline;
using std::pair;
using std::vector;

static bool running = true;
static vector<char*> binPaths;

struct Split
{
    string head;
    string tail;
};
