#include "osi.h"
#include "parsing.cpp"
#include "types.h"

#ifdef _WIN32

#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir

#else

#include <unistd.h>

#endif

string get_working_directory(bool omitHome)
{
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, FILENAME_MAX) == NULL)
    {
        cout << "Unable to determine working directory\n";
        return string();
    }
    else
    {
        string cwd = string(buffer);
        string homeDir = get_home_dir();
        if (omitHome && starts_with(cwd, homeDir))
        {
            return "~" + cwd.substr(homeDir.length());
        }

        return cwd;
    }
}

void change_working_directory(string path)
{
    if (starts_with(path, HOME_SYMBOL))
    {
        path = path.replace(0, 1, get_home_dir()).c_str();
    }

    chdir(path.c_str());
}
