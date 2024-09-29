#include "commands.cpp"
#include "startup.cpp"
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

void replace_input(string* input, string completion)
{
    if (!completion.empty())
    {
        input->erase(input->length() - Session.previous_completion.length());
        clear_characters(Session.previous_completion.length());

        cout << completion;
        input->append(completion);
        Session.previous_completion = completion;
    }
}

string read_input()
{
    string inputBuffer;
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
        // handle escape sequences
        // TEST: same for windows?
        else if (c == CH_ESC)
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
                        reset_completions(&Session);
                        clear_characters(inputBuffer.length());
                        inputBuffer = get_previous_entry(&Session);
                        cout << inputBuffer;
                        break;
                    case 'B':
                        reset_completions(&Session);
                        clear_characters(inputBuffer.length());
                        inputBuffer = get_next_entry(&Session);
                        cout << inputBuffer;
                        break;
                    case 'C':
                        // right: do nothing
                        break;
                    case 'D':
                        // left: do nothing
                        break;
                    case 'Z':
                        // shift + tab
                        string completion = get_completion(&Session,
                                                           inputBuffer,
                                                           false);
                        replace_input(&inputBuffer, completion);
                        break;
                }
            }
        }
        // DEL & BACKSPACE
        else if (c == CH_DEL)
        {
            if (!inputBuffer.empty())
            {
                // reset on input
                reset_completions(&Session);
                inputBuffer.erase(inputBuffer.length() - 1);
                clear_characters(1);
            }
        }
        // TAB
        else if (c == CH_TAB)
        {
            string completion = get_completion(&Session, inputBuffer, true);
            replace_input(&inputBuffer, completion);
        }
        else
        {
            // reset on input
            reset_completions(&Session);
            inputBuffer += c;
            cout << c;
        }
    }

    return inputBuffer;
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
