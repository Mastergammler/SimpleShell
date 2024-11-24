# Simple shell implementation

**Goals**
- Simple shell that runs on windows and linux
- Provide path auto complete for windows, including git completions

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
- [ ] Script execution delegation
     - [ ] .ps1 files
     - [ ] .bat files
     - [ ] .sh
- [ ] Env var handling
  - [ ] Echo variables
  - [ ] Apply variables to current execution

