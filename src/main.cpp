#include <iostream>
#include <string>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

int main()
{
    // Flush after every std::cout / std:cerr
    // TODO: why would you do this? Ease of use? Seems a bit odd
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Uncomment this block to pass the first stage
    cout << "$ ";

    string input;
    getline(cin, input);

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

    cout << commandName << ": command not found" << endl;
}
