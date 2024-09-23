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
    state->completion_index = 0;
    state->completions.clear();
    state->previous_completion = "";
    state->refresh_completions = true;
}

void set_current_completions(SessionState* state, vector<string> completions)
{
    state->completion_index = 0;
    state->completions = completions;
    state->refresh_completions = false;
}

// TODO: maybe switch to -1 based index?
// - because this feels quite akward
string get_next_completion(SessionState* state)
{
    assert(state->completion_index >= 0);
    if (state->completions.size() == 0) return "";

    if (state->completion_index >= state->completions.size())
    {
        // we're doing round about
        state->completion_index = 0;
    }

    return state->completions[state->completion_index++];
}

string get_previous_complotion(SessionState* state)
{
}
