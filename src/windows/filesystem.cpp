#include "../parsing.cpp"
#include "../types.h"
#include <fileapi.h>
#include <handleapi.h>
#include <minwinbase.h>
#include <string.h>
#include <windows.h>

void create_executable_name(string& searchName)
{
    Split extensionSplit = split_last(searchName, '.');
    if (!extensionSplit.found)
    {
        searchName.append(".exe");
    }
}

// TODO: handle env var better
string get_home_dir()
{
    return string(getenv("USERPROFILE"));
}

bool dir_exists(const char* absolutePath)
{
    DWORD attributes = GetFileAttributesA(absolutePath);
    return (attributes != INVALID_FILE_ATTRIBUTES &&
            (attributes & FILE_ATTRIBUTE_DIRECTORY));
}

bool file_exists(const char* path, const char* fileName)
{
    string completePath = string(path);
    completePath.append("/");
    completePath.append(fileName);

    DWORD attributes = GetFileAttributesA(completePath.c_str());

    // invalid path or no access
    if (attributes == INVALID_FILE_ATTRIBUTES) return false;

    // this is a directory
    if (attributes & FILE_ATTRIBUTE_DIRECTORY) return false;

    // simple check for executables
    const char* extension = strrchr(completePath.c_str(), '.');
    if (extension)
    {
        if (_stricmp(extension, ".exe") == 0 ||
            _stricmp(extension, ".com") == 0)
            return true;
    }

    // not a file we can execute with the 'system(path)' command
    return false;
}

bool file_exists(Split split)
{
    return file_exists(split.head.c_str(), split.tail.c_str());
}

vector<string> find_entries(const char* path, string startingWith)
{
    string searchGlob = string(path);
    if (startingWith.empty())
    {
        searchGlob += "\\*";
    }
    else
    {
        searchGlob += "\\" + startingWith + "*";
    }

    vector<string> entries;
    entries.reserve(16);

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchGlob.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) return entries;

    do
    {
        entries.push_back(findData.cFileName);
    }
    while (FindNextFileA(hFind, &findData) != NULL);

    FindClose(hFind);
    return entries;
}
