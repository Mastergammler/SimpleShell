#include "commands.cpp"
#include "startup.cpp"
#include "types.h"

void HandleCommand(Command cmd)
{
    auto it = BuiltinCommands.find(cmd.command_name);

    if (it != BuiltinCommands.end())
    {
        it->second(cmd);
    }
    else if (program_path(cmd.command_name) != NULL)
    {
        string completeCmd = cmd.command_name + " " + cmd.tail;
        system(completeCmd.c_str());
    }
    else
    {
        NotFound(cmd);
    }
}

void repl()
{
    cout << "$ ";

    string input;
    getline(cin, input);

    Split s = split_next(input, ' ');
    Command cmd = {s.head, s.tail};
    HandleCommand(cmd);
}

int main()
{
    // Flush after every std::cout / std:cerr
    // TODO: why would you do this? Ease of use? Seems a bit odd
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    Init_Builtins();
    Init_Path();

    while (running)
    {
        repl();
    }
}
