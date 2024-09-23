#include "commands.cpp"
#include "startup.cpp"
#include "state.cpp"
#include "types.h"
#include "unix/terminal.cpp"

static SessionState Session;

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
    Session.history_index = Session.history.size();
    reset_completions(&Session);

    while (true)
    {
        char c = unix_getch();
        if (c == '\n')
        {
            cout << endl;
            break;
        }
        // handle arrow keys
        // TODO: switch for windows
        else if (c == 27)
        {
            reset_completions(&Session);

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
                        input = get_previous_entry(&Session);
                        cout << input;
                        break;
                    case 'B':
                        clear_characters(input.length());
                        input = get_next_entry(&Session);
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
        else if (c == 127)
        {
            if (!input.empty())
            {
                // reset on input
                reset_completions(&Session);
                input.erase(input.length() - 1);
                clear_characters(1);
            }
        }
        // TAB
        else if (c == 9)
        {
            Split inputSplit = split_last(input, ' ');
            if (Session.refresh_completions)
            {
                PathSplit paths = resolve_absolute_path(inputSplit.tail);
                vector<string> currentCompletions = find_entries(
                                                        paths.path.c_str(),
                                                        paths.search_element);

                Session.previous_completion = paths.search_element;
                set_current_completions(&Session, currentCompletions);
            }

            // FIXME: BUG, when continuing pressing tab, it's completing the
            // next dir without a '/' in between
            string completion = get_next_completion(&Session);
            if (!completion.empty())
            {
                input.erase(input.length() -
                            Session.previous_completion.length());
                clear_characters(Session.previous_completion.length());

                cout << completion;
                input += completion;
                Session.previous_completion = completion;
            }
        }
        else
        {
            // reset on input
            reset_completions(&Session);
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
    Session.history.push_back(input);

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
