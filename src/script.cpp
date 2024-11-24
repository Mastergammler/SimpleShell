#include "parsing.cpp"
#include "types.h"

bool is_script(string scriptPath)
{
    if (ends_with(scriptPath, ".bat") || ends_with(scriptPath, ".ps1") ||
        ends_with(scriptPath, ".sh"))
    {
        return true;
    }
    return false;
}

string script_delegation_command(PathSplit path)
{
    string cmdPrepend;
    if (ends_with(path.search_element, ".bat"))
    {
        cmdPrepend = "cmd /c";
    }
    else if (ends_with(path.search_element, ".ps1"))
    {
        cmdPrepend = "powershell";
    }
    else if (ends_with(path.search_element, ".sh"))
    {
        const char* prefShell = getenv("SHELL");
        if (prefShell)
        {
            cmdPrepend = prefShell;
        }
        else
        {
            cmdPrepend = "bash";
        }
    }

    string completeCmd = cmdPrepend + " " + path.path + "/" +
                         path.search_element;
    return completeCmd;
}
