1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  So when you fork something, then it essentially sprouts a new process. The process you created from forking is the child, but the process that you forked from is the parent. Now you can run those two processes concurrently. But when you just call execvp() directly, then you do not get the advantage or the option to have processes concurrently run because it basically overwrites the current process.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  So fork() returns status codes depending on how it goes. If it returns -1, then there is a problem creating a child process. If it returns a 0, then a child process has been successfully created. How I did it was to receive the status code, store it in the variable, and then put it through if statements to check each status code, and print out the appropriate message.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  So execvp() uses the $PATH to help find out which command to execute. PATH, which is a system environment variable, basically tells the shell where to look for. For example, when you are trying to execvp on ls, PATH gives the shell a bunch of folders to look through that may have executable programs, including ls. It then goes through each folder trying to find the command to execute, until it finds it and then executes the command.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  After you fork, you have a parent and a child process. We want to call wait() so that the parent waits for the child process to finish in case we need the values produced from the child process. It also allows the parent to wait for the child to finish so that it can get the status code in case there are errors. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  Basically, WEXITSTATUS() gets the status code from the child process. And then this allows the parent to know the status code of the child, like in case it was a failure. Depending on how you want to want to react to a failure, you can try to make the process again, print error messages, or maybe clean up after the child's mess.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  So I have a couple of things. I kept a "quotedMode" boolean, and depending on if it is true or false as we go through each character of the command string, we would treat the spaces differently. If we are not in quoted mode and we see a space, then we would consider that as the end of the current argument and the beginning of the new argument. But if we are in quoted mode and we see a space, then we wouldn't register as the end of the current argument, that we don't tokenize it (yet). If we are in quoted mode and we see a quote, then we turn off quoted mode. If We aren't in quoted mode and we see a quote, then we turn on quoted mode. This is important because otherwise our echo wouldn't work properly, and we wouldn't capture multiple spaces if we wanted to try to display a message with multiple spaces.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  The biggest change was that I didn't use strtok to tokenize the string. Initially, I was going to do a strtok(" "), but that would mess with things that have multiple spaces, and that would mess with certain cases with echo. Another change was the inQuote command, which added an extra layer of trickiness for parsing the string correctly becauses spaces may or may not be the delimiter depending on if we are in quotes or not. Also, in general, just running into segfaults was annoying. Initially, I didn't know the purpose for storing a pointer to the buff for that struct, but I later realized that I could free it, which was the cause for one of those segfault errors.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are "soft" interrupts, meaning that when you are going through a process, it may interrupt the process and await a response. These can be generated by user processes or the kernel. Depending on the response it gets and the default reaction, it may cause the process to resume or not or do something different. Signals are a type of IPC, but these are more for notifying and/or controlling processes/logic rather than transferring/transmitting data than other types of IPCs like pipes.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: SIGKILL basically kills the process no matter what and doesn't care what the process says. It cannot be blocked or ignored. SIGTERM is the softer version of SIGKILL, and it can be blocked/ignored by the process if there is a response. But if there is no response, then it kills. SIGINT's primary purpose isn't to kill, but to interrupt the process. Then, the process can decide what response to take from there. If there is no response, then SIGINT would kill.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP pauses a process's execution, and it cannot be blocked/ignored by the process. It can't resume unless you do SIGCONT, which picks it back up. This is different than SIGINT because SIGINT can be blocked but SIGSTOP can't, and SIGINT wants to interrupt, but SIGSTOP wants to forceably pause the process.
