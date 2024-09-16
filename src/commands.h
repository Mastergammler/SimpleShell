#pragma once

#include <map>
#include <string>

using std::map;
using std::string;

struct Command
{
    string command_name;
    string tail;
};

typedef void (*CommandAction)(Command);

// TODO: FEATURES
//  - history (up and down arrow storage / session) -> stack?
void Builtin_Exit(Command cmd);
void Builtin_Echo(Command cmd);
void Builtin_Type(Command cmd);
void Builtin_Help(Command cmd);
void Builtin_Pwd(Command cmd);
void Builtin_Cd(Command cmd);
void NotFound(Command cmd);

static map<string, CommandAction> BuiltinCommands;
