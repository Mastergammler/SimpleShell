#pragma once

#include "types.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

// INPUT
char get_ch();

// CD
string get_home_dir();
string get_working_directory(bool omitHome = true);
void change_working_directory(string path);

// FIND FILES
bool dir_exists(const char* absolutePath);
bool file_exists(const char* path, const char* fileName);
bool file_exists(Split split);
vector<string> find_entries(const char* path, string startingWith);
