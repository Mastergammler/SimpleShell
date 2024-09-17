#include "filesystem.cpp"
#include "parsing.cpp"
#include "types.h"

// TODO: handle windows type paths correctly
bool check_suggestions(string inputDir)
{
    Split dirSplit = split_last_path(inputDir);

    string dirPath;
    string searchPart;

    // NOTE: root dir, linux only,
    // on windows we would always have the drive in front
    // so this would never apply
    if (dirSplit.head.length() == 0)
    {
        dirPath = '/';
        searchPart = dirSplit.tail;
    }
    // parent dir exists, also handles relative paths ./ & ../
    else if (dir_exists(dirSplit.head.c_str()))
    {
        dirPath = dirSplit.head;
        searchPart = dirSplit.tail;
    }
    // plain relative paths
    // TODO: for windows, this is not relevant again
    else if (!starts_with(dirSplit.head, '/'))
    {
        dirPath = get_working_directory(false);
        searchPart = dirSplit.head;
    }

    vector<string> entries = find_entries(dirPath.c_str(), searchPart);
    if (entries.size() > 0)
    {
        int i = 1;
        for (auto name : entries)
        {
            // FIXME: additional newline
            cout << name << "\t\t\t";
            if (i % 3 == 0) cout << "\n";
            i++;
        }
        cout << "\n";
    }

    return entries.size() > 0;
}
