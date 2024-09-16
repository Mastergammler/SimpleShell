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
        cerr << "Error opening directory: " << strerror(errno) << endl;
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

/**
 * CROSS PLATFORM
 * windows: <direct.h> -> _getcwd / _chdir
 * Unix: <unistd.h> getcwd / chdir
 */

string get_working_directory()
{
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, FILENAME_MAX) == NULL)
    {
        cout << "Unable to determine working directory\n";
        return string();
    }
    else
    {
        return string(buffer);
    }
}

void change_working_directory(string path)
{
    chdir(path.c_str());
}
