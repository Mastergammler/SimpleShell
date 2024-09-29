#include "parsing.cpp"
#include "state.cpp"
#include "types.h"
#include "unix/filesystem.cpp"

PathSplit resolve_absolute_path(string pathInput)
{
    Split dirSplit = split_last_path(pathInput);
    PathSplit path = {};
    path.trailing_path_slash = dirSplit.found;

    if (dir_exists(dirSplit.head.c_str()))
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

// TODO: switch -> git branch completion vs path completion
//  - is it akward if git would enable path completion completely?
//  - maybe i'll change it later to only be on certain commands
//  (like branch || checkout)
string get_completion(SessionState* session, string input, bool next = true)
{
    Split inputSplit = split_last(input, ' ');

    if (session->refresh_completions)
    {
        PathSplit paths = resolve_absolute_path(inputSplit.found ? inputSplit.tail
                                                                 : inputSplit.head);
        vector<string> currentCompletions = find_entries(paths.path.c_str(),
                                                         paths.search_element);

        session->previous_completion = paths.search_element;
        set_current_completions(session, currentCompletions);
    }

    if (next)
    {
        return get_next_completion(session);
    }
    else
    {
        return get_prev_completion(session);
    }
}

// TODO: windows: handle windows paths correctly
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
