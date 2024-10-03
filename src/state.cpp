#pragma once

#include "types.h"

string get_previous_entry(SessionState* state)
{
    if (state->history.size() == 0) return "";

    // get 'stuck' at earliest element
    state->history_index--;
    if (state->history_index < 0)
    {
        state->history_index = 0;
    }

    return state->history[state->history_index];
}

string get_next_entry(SessionState* state)
{
    state->history_index++;
    if (state->history_index >= state->history.size())
    {
        state->history_index = state->history.size();
        return "";
    }
    return state->history[state->history_index];
}

void reset_completions(CompletionCache* cache)
{
    cache->index = -1;
    cache->completions.clear();
    cache->prev_completion = "";
    cache->refresh = true;
}

void set_current_completions(CompletionCache* cache,
                             const vector<string> completions)
{
    cache->index = -1;
    cache->completions = completions;
    cache->refresh = false;
}

string get_next_completion(CompletionCache* cache)
{
    if (cache->completions.size() == 0) return "";
    assert(cache->index >= -1);

    int nextCmpIndex = cache->index + 1;
    if (nextCmpIndex >= cache->completions.size())
    {
        // going around
        nextCmpIndex = 0;
    }

    cache->index = nextCmpIndex;
    return cache->completions[nextCmpIndex];
}

string get_prev_completion(CompletionCache* cache)
{
    if (cache->completions.size() == 0) return "";

    // since size is a unsinged long i can not just check for < size,
    // i have to handle negative cases individually
    assert(cache->index < 0 || cache->index < cache->completions.size());

    int prevCmpIndex = cache->index - 1;
    if (prevCmpIndex < 0)
    {
        // going around
        prevCmpIndex = cache->completions.size() - 1;
    }

    cache->index = prevCmpIndex;
    return cache->completions[prevCmpIndex];
}
