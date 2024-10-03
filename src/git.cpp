#include "parsing.cpp"
#include "state.cpp"
#include "types.h"

// TODO: WIN32 _popen & _pclose are the windows equivalents
// need to be defined

string exec(const char* cmd)
{
    char buffer[128];
    string result;

    FILE* cmdOutput = popen(cmd, "r");
    if (!cmdOutput) return "";

    while (!feof(cmdOutput))
    {
        if (fgets(buffer, 128, cmdOutput) != NULL)
        {
            result.append(buffer);
        }
    }
    pclose(cmdOutput);
    return result;
}

vector<string> get_matching_branch_names(const string& searchString)
{
    string branches = exec("git branch -a");
    vector<string> allBranchNames = split_all(branches.c_str(), '\n');

    for (int i = 0; i < allBranchNames.size(); i++)
    {
        trim(allBranchNames[i], "*");
        trim_start(allBranchNames[i], "remotes/");
    }

    vector<string> matchingBranchNames;
    matchingBranchNames.reserve(allBranchNames.size());

    for (int i = 0; i < allBranchNames.size(); i++)
    {
        if (contains(allBranchNames[i], searchString))
        {
            matchingBranchNames.push_back(allBranchNames[i]);
        }
    }

    return matchingBranchNames;
}

string get_branch_completion(CompletionCache* completions,
                             const string input,
                             bool next = true)
{
    if (completions->refresh)
    {
        Split inputSplit = split_last(input, ' ');

        completions->prev_completion = inputSplit.tail;
        vector<string> branchNames = get_matching_branch_names(inputSplit.tail);
        set_current_completions(completions, branchNames);
    }

    if (next)
    {
        return get_next_completion(completions);
    }
    else
    {
        return get_prev_completion(completions);
    }
}
