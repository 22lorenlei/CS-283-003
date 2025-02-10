1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() gets your user input up until newline, which allows you to easily tell when it is the end of the command you want. It also handles EOF well and because you pass in a buffer size parameter, you don't have to worry about going over the buffer you read into.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  I don't see a difference for using malloc here vs a statically allocated array. When we use fgets, it takes in three paramters: the buffer we read it into, the max characters that we read, and stdin. This means that before the fgets line, we must already have decided the maximum size for the buffer, and since we know that it is SH_CMD_MAX, then we would just either malloc with SH_CMD_MAX * char size or just have buffer[SH_CMD_MAX]. Honestly, malloc is only useful if there is variable size that we don't know, and we have to free it in the end. However, we do know the exact size we should read, which is the SH_CMD_MAX. The statically allocated array is simpler, doesn't require to free it, and it works best with specified size limits upfront, which in this case it is.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming the spaces is important because it allows us to parse the command consistently. By having those extra spaces, we may not be able to recognize commands. Like if we do spaces with exit, it won't exit (the way I have it set up at least).

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  The first is output redirection (>). The second is appending output to a file (>>). The third is input redirection (<). What seems to be tricky is first figuring out where to store the output, so working with file descriptors is probably going to be hard. Also, if there are multiple redirections, then it could be difficult to make sure that each is evaluted correctly in order as well as that they get the correct input from one to another.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Pipes are used when you want to chain multiple commands. An example is like a cat file.txt | grep 'search'. However, redirection is used when you want to save command output to a file or the other way around, like using file contents as fodder for a command input. Example is like ls > file.txt, which puts your list of files onto a file.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: It is good practice to separate regular output (non errors) from actual error messages, which can help organize these messages since they all look similar from the terminal (in my opinion). Also, using 2> can be useful only because we separate stederr from stdout. This means that we can funnel only error messages to one file using 2> and regular output to another using >.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Like the previous, we should figure out how to separate stderr and stdout, at least visually. We should probably have some sort of system where we return error codes so like 0 would be good and anything else is problematic. I think merging them is fine since we could redirect stderr to stdout. We could detect the symbol for merging stdout and stderr and then launch code for it. Or maybe we could have stderr write to a file by default and have stdout write to another file by default, and then just combine those files together to read them out to stdout. 