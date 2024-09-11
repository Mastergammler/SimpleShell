#include <iostream>
#include <string>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

static bool running = true;

static const char* EXIT_COMMAND_NAME = "exit";

struct Command
{
    string command_name;
};

Command parseInput(string input)
{
    string commandName;
    int idx = input.find_first_of(" ");
    if (idx != string::npos)
    {
        commandName = input.substr(0, idx + 1);
    }
    else
    {
        commandName = input;
    }

    Command cmd = {commandName};
    return cmd;
}

void HandleCommand(Command cmd)
{
    if (cmd.command_name == EXIT_COMMAND_NAME)
    {
        running = false;
    }
    else
    {
        cout << cmd.command_name << ": command not found" << endl;
    }
}

void repl()
{
    cout << "$ ";

    string input;
    getline(cin, input);
    Command cmd = parseInput(input);
    HandleCommand(cmd);
}

int main()
{
    // Flush after every std::cout / std:cerr
    // TODO: why would you do this? Ease of use? Seems a bit odd
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (running)
    {
        repl();
    }
}
