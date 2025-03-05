#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

//  NOTES: Fix segfault error for too many commands (works every other time)
//         Conduct check for argument maxes
//         Do questions
 int exec_local_cmd_loop() {
    // So this is the max we are gonna have for our command (in length)
    char *cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
    if (cmd_buff == NULL) {
        return ERR_MEMORY;
    } 

    // This is our return code
    int rc = OK;

    command_list_t command_list;

    while (1) {
        // So this is essentially printing out dsh>
        printf("%s", SH_PROMPT);

        // Reads until EOF 
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // So if we enter nothing, then we set CMD_WARN_NO_CMD error
        if (strlen(cmd_buff) == 0) {
            printf(CMD_WARN_NO_CMD);
            rc = WARN_NO_CMDS;
        } else {
            rc = build_cmd_list(cmd_buff, &command_list);
            if (rc == ERR_TOO_MANY_COMMANDS) {
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                command_list.num = 0;
            } else if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
                printf("Too many arguments. Max is 7\n");
                command_list.num = 0;
            }
            // Check if command is Exit 
            else if (strcmp(command_list.commands[0].argv[0], "exit") == 0) {
                rc = OK;
                break;  
            
            // Dragon command
            } else if (strcmp(command_list.commands[0].argv[0], "dragon") == 0) {
                FILE *file = fopen("dragon.txt", "r");
                if (file == NULL) {
                    printf("Could not open file\n");
                    return -1;
                }
    
                char buffer[2056];
                while (fgets(buffer, sizeof(buffer), file) != NULL) {
                    printf("%s", buffer);
                }
                printf("\n");
    
                fclose(file);
                rc = OK;
            } else if (strcmp(command_list.commands[0].argv[0], "cd") == 0) {

                // cd with no arguments
                if (command_list.commands[0].argc == 1) {
                    rc = OK;
                    continue;
                
                // cd with one argument
                } else if (command_list.commands[0].argc == 2) {

                    if (chdir(command_list.commands[0].argv[1]) == -1) {
                        printf("Could not get into folder\n");
                    }
                
                // If we have cd with any other arguments, then 
                } else {
                    printf("cd takes one argument only\n");
                }
            
            // If it is not those two special commands, then run it normally
            } else if (rc == OK) {
                int status = execute_pipeline(&command_list);
                rc = status;
            }
            else {
                printf("SOME UNKNOWN ERROR\n");
            }
        }
    }
    // Free memory
    free(cmd_buff);
    // If we ended up having something to parse, we free this too
    for (int i = 0; i < command_list.num; i++) {
        free(command_list.commands[i]._cmd_buffer);
    }
    return rc;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {

    int isInQuoteMode = 0;
    char currentQuote = '\0';

    // Array of pointers to command
    char *pointerArray[CMD_MAX];

    // Clean up the beginning whitespace
    while (*cmd_line && isspace((unsigned char) *cmd_line)) {
        cmd_line++;
    }

    int commandCount = 0;
    pointerArray[commandCount] = cmd_line;
    commandCount++;

    while (*cmd_line) {

        // Command max check
        if (commandCount > CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
        
        if (isInQuoteMode) {

            // Closing quotes and end quote mode
            if (*cmd_line == currentQuote) {
                *cmd_line = '\0';  
                isInQuoteMode = 0;
                currentQuote = '\0'; 
            }
        } else {

            // Turning on quote mode
            if (*cmd_line == '"' || *cmd_line == '\'') {
                isInQuoteMode = 1;
                currentQuote = *cmd_line;
            
                // New command cause pipe
            } else if (*cmd_line == '|') {
                pointerArray[commandCount] = cmd_line + 1;
                commandCount++;
                cmd_line++;
            }
        }
        cmd_line++;
    }

    // Null terminate it because we need this for execvp
    pointerArray[commandCount] = NULL;

    // For loop and call helper on each of the commands
    for (int i = 0; i < commandCount; i++) {
        cmd_buff_t cmd_buff_instance;
        int status = build_cmd_buff(pointerArray[i], &cmd_buff_instance);
        if (status != OK) {
            return status;
        }
        clist->commands[i] = cmd_buff_instance;
    }

    clist->num = commandCount;

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    int argumentCount = 0;
    char *cmdLineCopy = strdup(cmd_line);
    if (cmdLineCopy == NULL) {
        return ERR_MEMORY;  
    }

    char *copyHead = cmdLineCopy; 

    // Extra just in case we go above (but we won't cause we have a max check)
    char *pointerArray[CMD_ARGV_MAX + 3];  
    int isInQuoteMode = 0;
    char currentQuote = '\0';
    char *currentArgumentPointer = NULL;

    while (*cmdLineCopy) {
        if (argumentCount >= CMD_ARGV_MAX - 1) {
            free(copyHead);
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        
        // Similar logic above function with the quotes
        if (isInQuoteMode) {

            if (*cmdLineCopy == currentQuote) {
                *cmdLineCopy = '\0';
                isInQuoteMode = 0;
                currentQuote = '\0'; 
            }
        } else {

            if (*cmdLineCopy == '"' || *cmdLineCopy == '\'') {
                isInQuoteMode = 1;
                currentQuote = *cmdLineCopy; 
                currentArgumentPointer = cmdLineCopy + 1; 
            
            } else if (isspace((unsigned char)*cmdLineCopy)) { 

                // New argument
                if (currentArgumentPointer != NULL) {
                    *cmdLineCopy = '\0';  
                    pointerArray[argumentCount] = currentArgumentPointer;
                    argumentCount++;
                    currentArgumentPointer = NULL;
                }
                
                // Ignore basically
            } else if (*cmdLineCopy == '|') {
                break;
            } else {
                if (currentArgumentPointer == NULL) {
                    currentArgumentPointer = cmdLineCopy;
                }
            }
        }
        cmdLineCopy++;
    }

    // Ending
    if (currentArgumentPointer != NULL) {
        pointerArray[argumentCount] = currentArgumentPointer;
        argumentCount++;
    }

    // Puts the arguments in the struct
    for (int i = 0; i < argumentCount; i++) {
        cmd_buff->argv[i] = pointerArray[i];
    }

    // Null terminate
    cmd_buff->argv[argumentCount] = NULL;  
    cmd_buff->argc = argumentCount;
    cmd_buff->_cmd_buffer = copyHead; 

    return OK;
}


// FROM the fork-pipeline.c example document and some modificatinos
int execute_pipeline(command_list_t *clist) {
    int num_commands = clist->num;

    int pipes[num_commands - 1][2];  // Array of pipes
    pid_t pids[num_commands];        // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    int isError = OK;

    // Create processes for each command
    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {  // Child process
            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Set up output pipe for all except last process
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            free(clist->commands[i]._cmd_buffer);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
    return isError;
}
