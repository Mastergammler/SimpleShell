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

void reset_completions(SessionState* state)
{
    state->completion_index = -1;
    state->completions.clear();
    state->previous_completion = "";
    state->refresh_completions = true;
}

void set_current_completions(SessionState* state, vector<string> completions)
{
    state->completion_index = -1;
    state->completions = completions;
    state->refresh_completions = false;
}

string get_next_completion(SessionState* state)
{
    if (state->completions.size() == 0) return "";
    assert(state->completion_index >= -1);

    int nextCmpIndex = state->completion_index + 1;
    if (nextCmpIndex >= state->completions.size())
    {
        // going around
        nextCmpIndex = 0;
    }

    state->completion_index = nextCmpIndex;
    return state->completions[nextCmpIndex];
}

string get_prev_completion(SessionState* state)
{
    if (state->completions.size() == 0) return "";

    // since size is a unsinged long i can not just check for < size,
    // i have to handle negative cases individually
    assert(state->completion_index < 0 ||
           state->completion_index < state->completions.size());

    int prevCmpIndex = state->completion_index - 1;
    if (prevCmpIndex < 0)
    {
        // ground around
        prevCmpIndex = state->completions.size() - 1;
    }

    state->completion_index = prevCmpIndex;
    return state->completions[prevCmpIndex];
}
