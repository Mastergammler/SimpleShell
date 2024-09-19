#include "commands.cpp"
#include "startup.cpp"
#include "types.h"
#include "unix/terminal.cpp"

static vector<string> history;
static int historyIndex;

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

string get_previous_entry()
{
    if (history.size() == 0) return "";

    // get 'stuck' at earliest element
    historyIndex--;
    if (historyIndex < 0)
    {
        historyIndex = 0;
    }

    return history[historyIndex];
}

string get_next_entry()
{
    historyIndex++;
    if (historyIndex >= history.size())
    {
        historyIndex = history.size();
        return "";
    }
    return history[historyIndex];
}

void clear_characters(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        // clears 1 character & moves cursor position
        cout << "\b \b";
    }
}

string read_input()
{
    string input;
    historyIndex = history.size();

    while (true)
    {
        char c = unix_getch();
        if (c == '\n')
        {
            cout << endl;
            break;
        }
        // handle arrow keys
        else if (c == 27)
        {
            // termios returns a secape sequence instead of single characters
            char seq[3];
            seq[0] = unix_getch();
            seq[1] = unix_getch();
            seq[2] = '\0';

            if (seq[0] == '[')
            {
                switch (seq[1])
                {
                    case 'A':
                        clear_characters(input.length());
                        input = get_previous_entry();
                        cout << input;
                        break;
                    case 'B':
                        clear_characters(input.length());
                        input = get_next_entry();
                        cout << input;
                        break;
                    case 'C':
                        // right: do nothing
                        break;
                    case 'D':
                        // left: do nothing
                        break;
                }
            }
        }
        // DEL & BACKSPACE
        else if (c == 127 || c == 0)
        {
            if (!input.empty())
            {
                input.erase(input.length() - 1);
                clear_characters(1);
            }
        }
        // TAB
        else if (c == 9)
        {
            // TODO: get actual file completion
            //- based on current input
            //- parse out the path till here
            //- get the next entries
            string completion = " it's magic ";
            cout << completion;
            input += completion;
        }
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
    history.push_back(input);

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
