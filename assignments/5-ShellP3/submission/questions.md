1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

By using the waitpid, they can ensure that all child processes are complete. Essentially, the for loop that contains the waitpid essnetially applies waitpid to each of the child processes spawned. It then lets the parent know to wait until the child is done, either by terminating or sending a signal. If you forget to call waitpid, the child process will be zombified and adds overhead to the OS since it doesn't get cleaned up. 

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

dup2 duplicates file descriptors. We need to close unused pipe ends after calling dup2 because we don't want to run out of available file descriptors. If we keep leaving them open, then the program can have some problems when trying to open new pipes or files.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

execvp essentially tries to find executable files in the directories in the PATH environment variable and searches through each folder until it gets a match. But you won't be able to find such built in commands there (ex. cd). Also, a possible challenge if it were to be implemented is as an external process would be that since execvp creates a new child process in that case, things that happen in the child process won't transfer to the parent. So if you cd, then you'd be in the right directory in the child process, but not in the parent process.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_answer here_
