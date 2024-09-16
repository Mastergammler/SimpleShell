#pragma once

#include "commands.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <map>
#include <vector>

/**
 * UNIX Headers
 */
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

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
