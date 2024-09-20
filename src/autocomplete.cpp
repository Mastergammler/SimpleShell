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
        path.file_name = dirSplit.tail;
    }
    // parent dir exists, also handles relative paths ./ & ../
    else if (dir_exists(dirSplit.head.c_str()))
    {
        path.path = dirSplit.head;
        path.file_name = dirSplit.tail;
    }
    // plain relative paths
    // TODO: for windows, this is not relevant again
    else if (!starts_with(dirSplit.head, '/'))
    {
        path.path = get_working_directory(false);
        path.file_name = dirSplit.head;
    }

    return path;
}

// TODO:
//  handle home dir
//  handle root dir
//  handle next/previous element selection
string get_completion(string input)
{
    // only want to complete at the last element
    Split inputSplit = split_last(input, ' ');
    PathSplit split = resolve_absolute_path(inputSplit.tail);

    string match = find_next_entry(split.path.c_str(), split.file_name);

    if (match.empty()) return input;

    string wd = get_working_directory(false);
    if (starts_with(split.path, wd))
    {
        split.path = split.path.substr(wd.length());
    }

    if (!split.path.empty())
    {
        split.path += "/";
    }

    return inputSplit.head + " " + split.path + match;
}

// TODO: handle windows type paths correctly
bool print_suggestions(string inputDir)
{
    PathSplit split = resolve_absolute_path(inputDir);
    vector<string> entries = find_entries(split.path.c_str(), split.file_name);

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
