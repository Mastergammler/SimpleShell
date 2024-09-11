#include <iostream>
#include <string>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

static bool running = true;

static const char* EXIT_CMD_NAME = "exit";
static const char* ECHO_CMD_NAME = "echo";

struct Command
{
    string command_name;
    string tail;
};

Command parseInput(string input)
{
    Command cmd = {};
    int idx = input.find_first_of(" ");
    if (idx != string::npos)
    {
        cmd.command_name = input.substr(0, idx);
        cmd.tail = input.substr(idx + 1);
    }
    else
    {
        cmd.command_name = input;
    }

    return cmd;
}

void HandleCommand(Command cmd)
{
    if (cmd.command_name == EXIT_CMD_NAME)
    {
        running = false;
    }
    else if (cmd.command_name == ECHO_CMD_NAME)
    {
        cout << cmd.tail << endl;
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
