#include "../parsing.cpp"
#include "types.h"

bool is_executable(const char* path, const char* fileName)
{
    string fullPath = string(path) + "/" + string(fileName);

    struct stat st;
    if (stat(fullPath.c_str(), &st) < 0)
    {
        return false;
    }
    return (st.st_mode & S_IXUSR) && S_ISREG(st.st_mode);
}

bool file_exists(const char* path, const char* fileName)
{
    DIR* dir = opendir(path);
    if (dir == NULL)
    {
        // cerr << "Error opening directory: " << strerror(errno) << endl;
        return false;
    }

    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, fileName) == 0 &&
            is_executable(path, entry->d_name))
        {
            closedir(dir);
            return true;
        }
    }

    closedir(dir);
    return false;
}

vector<string> find_entries(const char* path, string startingWith)
{
    vector<string> entries;
    DIR* dir = opendir(path);
    if (dir == NULL) return entries;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (starts_with(entry->d_name, startingWith))
        {
            entries.push_back(entry->d_name);
        }
    }

    return entries;
}

string find_next_entry(const char* path, string startingWith, int skipElements)
{
    DIR* dir = opendir(path);
    if (dir == NULL) return "";

    int curIndx = 0;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (starts_with(entry->d_name, startingWith) &&
            skipElements == curIndx++)
            return entry->d_name;
    }

    return "";
}

// TODO: if i think it exists directly, can't i check it directly?
//-> do i really need to iterate through all the directories?
bool file_exists(Split split)
{
    return file_exists(split.head.c_str(), split.tail.c_str());
}

bool dir_exists(const char* absolutePath)
{
    DIR* dir = opendir(absolutePath);
    if (dir == NULL)
    {
        return false;
    }

    closedir(dir);
    return true;
}

// TODO: is the var diferent on windows?
string get_home_dir()
{
    return string(getenv("HOME"));
}
