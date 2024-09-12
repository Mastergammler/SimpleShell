#include "commands.cpp"
#include "types.h"

void InitBuiltins()
{
    BuiltinCommands["exit"] = Builtin_Exit;
    BuiltinCommands["echo"] = Builtin_Echo;
    BuiltinCommands["type"] = Builtin_Type;
}

void HandleCommand(Command cmd)
{
    auto it = BuiltinCommands.find(cmd.command_name);

    if (it != BuiltinCommands.end())
    {
        it->second(cmd);
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

    InitBuiltins();

    while (running)
    {
        repl();
    }
}
