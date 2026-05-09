# Files Watcher
This is C program which allow user to monitor the files and Directories of the linux file system using the system calls like IN_ACCESS , IN_CREATE and other from "inotify" , this allow use to keep logs of what happen in the file system

## Pointers to note

- The C program uses <b> inotify.h </b> to access system calls
- The program also print which file or Directory was created and at what time along with other system calls
- The program is writen for linux only
- It do not save any data/logs in a file , it just print them on termainal 
- It only capture system calls only of the Directory it is running and its sub Directories 
- Compile it using GCC for linux 

## Reference
- man pages of read , open , inotify and other
- can also use website: https://man7.org/ for reference
- for system calls check <b> inotify.h </b> file ( shortcut: CTRL + LEFT CLICK in vscode , on #include <sys/inotify.h>)