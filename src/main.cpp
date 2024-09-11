#include <iostream>

int main()
{
    // Flush after every std::cout / std:cerr
    // TODO: why would you do this? Ease of use? Seems a bit odd
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Uncomment this block to pass the first stage
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
}
