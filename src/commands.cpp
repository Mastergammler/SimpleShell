#include "filesystem.cpp"
#include "parsing.cpp"
#include "types.h"

void Builtin_Exit(Command cmd)
{
    running = false;
}

void Builtin_Echo(Command cmd)
{
    cout << cmd.tail << endl;
}

void Builtin_Type(Command cmd)
{
    Split next = split_next(cmd.tail, ' ');

    string arg1 = next.head;

    auto it = BuiltinCommands.find(arg1);
    if (it != BuiltinCommands.end())
    {
        cout << next.head << " is a shell builtin" << endl;
    }
    else
    {
        for (int i = 0; i < binPaths.size(); i++)
        {
            const char* curPath = binPaths[i];

            if (file_exists(curPath, arg1.c_str()))
            {
                cout << arg1 << " is " << curPath << "/" << arg1 << endl;
                return;
            }
        }

        cout << arg1 << ": not found" << endl;
    }
}

void Builtin_Help(Command cmd)
{
    cout << "The following builtins are available:\n";

    for (auto it = BuiltinCommands.begin(); it != BuiltinCommands.end(); ++it)
    {
        cout << " -> " << it->first << " is a shell builtin\n";
    }

    cout << "Bin paths are:\n";

    for (int i = 0; i < binPaths.size(); i++)
    {
        cout << "  " << binPaths[i] << "\n";
    }
}

void NotFound(Command cmd)
{
    cout << cmd.command_name << ": command not found" << endl;
}
