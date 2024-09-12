#pragma once

#include "commands.h"
#include <iostream>
#include <map>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::pair;

static bool running = true;

struct Split
{
    string head;
    string tail;
};
