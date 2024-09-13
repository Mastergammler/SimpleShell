#include "commands.h"
#include "parsing.cpp"
#include "types.h"

void Init_Builtins()
{
    BuiltinCommands["exit"] = Builtin_Exit;
    BuiltinCommands["echo"] = Builtin_Echo;
    BuiltinCommands["type"] = Builtin_Type;
    BuiltinCommands["help"] = Builtin_Help;
}

void Init_Path()
{
    const char* pathValue = getenv("PATH");
    if (pathValue != NULL)
    {
        // TODO: on windows the separator is different
        binPaths = split_all(pathValue, ':');
    }
}
