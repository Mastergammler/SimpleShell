#include "types.h"
#include <sys/stat.h>

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
