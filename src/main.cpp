#include "commands.cpp"
#include "git.cpp"
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

void replace_input(CompletionCache* cache, string* input, string completion)
{
    if (!completion.empty())
    {
        input->erase(input->length() - cache->prev_completion.length());
        clear_characters(cache->prev_completion.length());

        cout << completion;
        input->append(completion);
        cache->prev_completion = completion;
    }
    else
    {
        cout << SOUND;
    }
}

void reset_all_completions()
{
    reset_completions(&Session.path_completions);
    reset_completions(&Session.branch_completions);
}

string read_input()
{
    string inputBuffer;
    Session.history_index = Session.history.size();
    reset_all_completions();

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
        // TODO: refactor, what to do functions (on_arrow_up_ress) etc
        // and when to trigger it
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
                    case ESCS_ARROW_UP:
                        reset_all_completions();
                        clear_characters(inputBuffer.length());
                        inputBuffer = get_previous_entry(&Session);
                        cout << inputBuffer;
                        break;
                    case ESCS_ARROW_DOWN:
                        reset_all_completions();
                        clear_characters(inputBuffer.length());
                        inputBuffer = get_next_entry(&Session);
                        cout << inputBuffer;
                        break;
                    case ESCS_ARROW_RIGHT:
                        // right: do nothing
                        break;
                    case ESCS_ARROW_LEFT:
                        // left: do nothing
                        break;
                    // DEL has sequence 3~
                    case ESCS_DEL_1:
                    {
                        char next = unix_getch();
                        if (next == ESCS_DEL_2)
                        {
                            reset_all_completions();
                            clear_characters(inputBuffer.length());
                            inputBuffer = "";
                        }
                    }
                    break;
                    case ESCS_SHIFT_TAB:
                        // TODO: git branch completions

                        // shift + tab
                        string completion = get_path_completion(&Session.path_completions,
                                                                inputBuffer,
                                                                false);
                        replace_input(&Session.path_completions,
                                      &inputBuffer,
                                      completion);
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
                reset_all_completions();
                inputBuffer.erase(inputBuffer.length() - 1);
                clear_characters(1);
            }
        }
        // TAB
        else if (c == CH_TAB)
        {
            Split inputSplit = split_next(inputBuffer, ' ');

            // TODO: there can be multiple git contexts where i also would have
            // file search -> i have to differentiate those later
            if (inputSplit.head == "git")
            {
                string completion = get_branch_completion(
                                                        &Session.branch_completions,
                                                        inputBuffer,
                                                        true);
                replace_input(&Session.branch_completions,
                              &inputBuffer,
                              completion);
            }
            else
            {
                string completion = get_path_completion(&Session.path_completions,
                                                        inputBuffer,
                                                        true);
                replace_input(&Session.path_completions,
                              &inputBuffer,
                              completion);
            }
        }
        else
        {
            // reset on input
            reset_all_completions();
            inputBuffer += c;
            cout << c;
        }
    }

    return inputBuffer;
}

void repl()
{
    cout << BOLD << COLOR_BLUE << get_working_directory() << ANSI_RESET
         << " # ";

    string input = read_input();
    Session.history.push_back(input);

    Split s = split_next(input, ' ');
    Command cmd = {s.head, s.tail};
    HandleCommand(cmd);
}

int main()
{
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    Init_Builtins();
    Init_Path();

    while (running)
    {
        repl();
    }
}
