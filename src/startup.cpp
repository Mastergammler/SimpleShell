#include "commands.h"
#include "osi.h"
#include "parsing.cpp"
#include "types.h"

void Init_Builtins()
{
    BuiltinCommands["exit"] = Builtin_Exit;
    BuiltinCommands["echo"] = Builtin_Echo;
    BuiltinCommands["type"] = Builtin_Type;
    BuiltinCommands["help"] = Builtin_Help;
    BuiltinCommands["pwd"] = Builtin_Pwd;
    BuiltinCommands["cd"] = Builtin_Cd;
    BuiltinCommands["test"] = Debug_Test;
}

void Init_Path()
{
    // path for executables to search first in
    // especially important for windows, because usually the system32 paths will
    // always take precidence
    const char* priorityPath = getenv("MGS_PATH");
    if (priorityPath != NULL)
    {
        binPaths = split_all(priorityPath, PATH_SEPARATOR);
    }

    const char* pathValue = getenv("PATH");
    if (pathValue != NULL)
    {
        vector<string> searchPaths = split_all(pathValue, PATH_SEPARATOR);
        for (int i = 0; i < searchPaths.size(); i++)
        {
            binPaths.push_back(searchPaths[i]);
        }
    }
}
