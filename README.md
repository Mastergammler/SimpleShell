# Simple shell implementation

**Goals**
- Simple shell that runs on windows and linux
- Provide path auto complete for windows, including git completions

**Env Vars**

`$PATH` - used to search executables in
`$MGS_PATH` - additional path, that takes precidence over variables defined in `$PATH` (is searched first)
`$SHELL` - name or path of the executable to use for `.sh` files

**Todo**

*Autocomplete*
- [x] relative paths
- [x] Tab autocomplete loop
- [x] case insensitive path match
- [x] Autocomplete on tab
    - [x] Handle realative paths
    - [x] Handle absolute path completions (unix)
    - [x] Handle home dir completions
- [ ] Git completions
    - [x] Branch completion
    - [ ] Switch branch / file completions 

*Windows*
- [x] Windows build setup
- [x] Windows filesystem handling
- [x] Windows read key handling
- [x] Windows path variable handling
- [ ] Key Issues to Fix
     - [ ] Shift + Tab detection
     - [x] Ctrl + C usage

*Features*
- [x] Script execution delegation
     - [x] .ps1 files
     - [x] .bat files
     - [x] .sh
- [ ] Env var handling
  - [ ] Echo variables
  - [ ] Apply variables to current execution

