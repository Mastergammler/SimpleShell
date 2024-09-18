#include "commands.cpp"
#include "startup.cpp"
#include "types.h"
#include "unix/terminal.cpp"

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

string read_input()
{
    string input;

    while (true)
    {
        char c = unix_getch();
        if (c == '\n')
        {
            cout << endl;
            break;
        }

        // DEL & BACKSPACE
        else if (c == 127 || c == 0)
        {
            // FIXME: deeply flawed, beacuse messes up screen manipulation
            //- when moving with cursor, because then it'll never be empty
            if (!input.empty())
            {
                input.erase(input.length() - 1);
                // moving cursor position
                cout << "\b \b";
            }
        }
        // TAB
        else if (c == 9)
        {
            string completion = " it's magic ";
            cout << completion;
            input += completion;
        }
        // TODO: handle arrow keys -> to not move off the line
        else
        {
            input += c;
            cout << c;
        }
    }

    return input;
}

void repl()
{
    cout << get_working_directory() << " # ";

    string input = read_input();

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
