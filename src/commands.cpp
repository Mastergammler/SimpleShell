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

    auto it = BuiltinCommands.find(next.head);
    if (it != BuiltinCommands.end())
    {
        cout << next.head << " is a shell builtin" << endl;
    }
    else
    {
        cout << next.head << ": not found" << endl;
    }
}

void NotFound(Command cmd)
{
    cout << cmd.command_name << ": command not found" << endl;
}
