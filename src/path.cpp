#include "osi.h"
#include "parsing.cpp"
#include "state.cpp"
#include "types.h"

PathSplit resolve_absolute_path(string pathInput)
{
    if (starts_with(pathInput, HOME_SYMBOL))
    {
        string homeDir = get_home_dir();
        pathInput.replace(0, 1, homeDir);
    }

    Split dirSplit = split_last_path(pathInput);
    PathSplit path = {};

    // TODO: trailing slash handling
    // - for the case of 2 completions after each other
    path.trailing_path_slash = dirSplit.found;

    // absolute path handling
    if (dir_exists(dirSplit.head.c_str()))
    {
        path.path = dirSplit.head;
        path.search_element = dirSplit.tail;
    }
    // relative path check
    // TODO: WIN - check for drive letter
    else if (!starts_with(dirSplit.head, '/'))
    {
        path.path = get_working_directory(false);
        path.search_element = dirSplit.head;
    }

    return path;
}

string get_path_completion(CompletionCache* session,
                           string input,
                           bool next = true)
{
    Split inputSplit = split_last(input, ' ');

    if (session->refresh)
    {
        PathSplit paths = resolve_absolute_path(inputSplit.found ? inputSplit.tail
                                                                 : inputSplit.head);
        vector<string> currentCompletions = find_entries(paths.path.c_str(),
                                                         paths.search_element);

        session->prev_completion = paths.search_element;
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
