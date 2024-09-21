#include "parsing.cpp"
#include "types.h"
#include "unix/filesystem.cpp"

PathSplit resolve_absolute_path(string pathInput)
{
    Split dirSplit = split_last_path(pathInput);
    PathSplit path = {};

    // NOTE: root dir, linux only,
    // on windows we would always have the drive in front
    // so this would never apply
    if (dirSplit.head.length() == 0)
    {
        path.path = '/';
        path.search_element = dirSplit.tail;
    }
    // parent dir exists, also handles relative paths ./ & ../
    else if (dir_exists(dirSplit.head.c_str()))
    {
        path.path = dirSplit.head;
        path.search_element = dirSplit.tail;
    }
    // plain relative paths
    // TODO: for windows, this is not relevant again
    else if (!starts_with(dirSplit.head, '/'))
    {
        path.path = get_working_directory(false);
        path.search_element = dirSplit.head;
    }

    return path;
}

// TODO:
//  handle home dir
//  handle root dir
Completion get_completion(string input, int skipElements = 0)
{
    Completion cmp = {};
    // only want to complete at the last element
    PathSplit split = resolve_absolute_path(input);

    string match;
    if (skipElements > 0)
    {
        // TODO: create: finde idx entry after match
        match = find_next_entry(split.path.c_str(), "", skipElements);
    }
    else
    {
        match = find_next_entry(split.path.c_str(),
                                split.search_element,
                                skipElements);
    }

    // TODO: When the last char is a /, then i change dir
    //- else i search next element in this dir

    if (match.empty()) return cmp;

    string wd = get_working_directory(false);
    if (starts_with(split.path, wd))
    {
        split.path = split.path.substr(wd.length());
    }

    if (!split.path.empty())
    {
        split.path += "/";
    }

    // TODO: dir changed
    cmp.text = split.path + match;
    cmp.found = true;

    return cmp;
}

// TODO: handle windows type paths correctly
bool print_suggestions(string inputDir)
{
    PathSplit split = resolve_absolute_path(inputDir);
    vector<string> entries = find_entries(split.path.c_str(),
                                          split.search_element);

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
