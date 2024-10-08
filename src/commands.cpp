#include "path.cpp"
#include "types.h"

void Builtin_Exit(Command cmd)
{
    running = false;
}

void Builtin_Echo(Command cmd)
{
    cout << cmd.tail << endl;
}

const char* program_path(string name)
{
    for (int i = 0; i < binPaths.size(); i++)
    {
        const char* curPath = binPaths[i].c_str();

        if (file_exists(curPath, name.c_str()))
        {
            return curPath;
        }
    }

    return NULL;
}

void Builtin_Type(Command cmd)
{
    Split next = split_next(cmd.tail, ' ');

    string arg1 = next.head;

    auto it = BuiltinCommands.find(arg1);
    if (it != BuiltinCommands.end())
    {
        cout << next.head << " is a shell builtin" << endl;
    }
    else
    {
        const char* foundPath = program_path(arg1);
        if (foundPath != NULL)
        {
            cout << arg1 << " is " << foundPath << "/" << arg1 << endl;
        }
        else
        {
            cout << arg1 << ": not found" << endl;
        }
    }
}

void Builtin_Help(Command cmd)
{
    cout << "The following builtins are available:\n";

    for (auto it = BuiltinCommands.begin(); it != BuiltinCommands.end(); ++it)
    {
        cout << " -> " << it->first << " is a shell builtin\n";
    }

    cout << "Bin paths are:\n";

    for (int i = 0; i < binPaths.size(); i++)
    {
        cout << "  " << binPaths[i] << "\n";
    }
}

void Builtin_Pwd(Command cmd)
{
    cout << get_working_directory() << "\n";
}

// TODO: feature, soft autocomplete
//-> check the dirs, if it has one starting with that letters etc
//-> like closest matches etc
void Builtin_Cd(Command cmd)
{
    const char* dir = cmd.tail.c_str();
    if (starts_with(cmd.tail, HOME_SYMBOL))
    {
        dir = cmd.tail.replace(0, 1, get_home_dir()).c_str();
    }

    // TODO: this seems like custom generalized path completion logic
    //  -> ill need to separate these somewhere
    //  => To have this not all bound to the cd command
    if (dir_exists(dir))
    {
        change_working_directory(dir);
    }
    else
    {
        Split fileSplit = split_last_path(dir);
        if (file_exists(fileSplit))
        {
            cout << dir << ": Is a file!\n";
        }
        else
        {
            if (!print_suggestions(dir))
                cout << dir << ": No such file or directory\n";
        }
    }
}

void Debug_PrintAscii(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        int ascii = s[i];
        cout << ascii << " ";
        if (ascii == 10)
        {
            cout << "\n";
        }
    }
}

void Debug_Test(Command cmd)
{
    cout << "\nRoot dir exists? " << dir_exists("/");

    vector<string> entries = find_entries("/", "");

    for (int i = 0; i < entries.size(); i++)
    {
        cout << "\n  " << entries[i];
    }

    cout << endl;
    // cout << "no test implemented";
}

void NotFound(Command cmd)
{
    cout << cmd.command_name << ": command not found" << endl;
}
