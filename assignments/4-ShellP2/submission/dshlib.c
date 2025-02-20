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
int exec_local_cmd_loop() {
    // So this is the max we are gonna have for our command (in length)
    char *cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
    if (cmd_buff == NULL) {
        return ERR_MEMORY;
    } 

    // This is our return code
    int rc = OK;

    // Our list of commands essentially
    cmd_buff_t cmd;

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
            rc = build_cmd_buff(cmd_buff, &cmd);

                // Check if command is Exit 
            if (strcmp(cmd.argv[0], "exit") == 0) {
                rc = OK;
                break;
            
                // dragon
            } else if (strcmp(cmd.argv[0], "dragon") == 0){
                rc = OK;
                print_dragon();
            // Check if command is cd
            } else if (strcmp(cmd.argv[0], "cd") == 0) {

                // cd with no arguments
                if (cmd.argc == 1) {
                    rc = OK;
                    continue;
                
                // cd with one argument
                } else if (cmd.argc == 2) {

                    // cd into the folder, which we assume is the second argument
                    int status = chdir(cmd.argv[1]);

                    // In case we have like no permissions to that folder
                    if (status == -1) {
                        printf("Could not get into folder\n");
                    }
                
                // If we have cd with any other arguments, then 
                } else {
                    printf("cd takes one argument only\n");
                }
            
            // If it is not those two special commands, then run it normally
            } else if (rc == OK) {
                pid_t pid = fork();
                if (pid == 0) {
                    int status = execvp(cmd.argv[0], cmd.argv);
                    if (status == -1) {
                        rc = ERR_EXEC_CMD;
                        break;
                    }
                } else if (pid < 0) {
                    printf("Child Problems coming from Fork");
                    rc = ERR_EXEC_CMD;
                    break;
                // Wait for child to sync up
                } else {
                    wait(NULL);
                }
            }
            else {
                printf("SOME UNKNOWN ERROR\n");
            } 
        }
    
    }

    // Free memory
    free(cmd_buff);

    // If we ended up having something to parse, we free this too
    if (cmd._cmd_buffer != NULL) {
        free(cmd._cmd_buffer);
        cmd._cmd_buffer = NULL;
    }
    return rc;
}


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    int argumentCount = 0;
    char *cmdLineCopy = strdup(cmd_line);
    if (cmdLineCopy == NULL) {
        return ERR_MEMORY;  
    }

    // For when we need to free later
    char *copyHead = cmdLineCopy; 

    // Pointer locations for each argument
    char *pointerArray[CMD_ARGV_MAX];  
    int isInQuoteMode = 0;
    char currentQuote = '\0';
    char *currentArgumentPointer = NULL;

    while (*cmdLineCopy) {

        // If we are in quotes
        if (isInQuoteMode) {

            // If our opening quote matches our current character which is the closing match to our opening quote
            // Turn it off
            if (*cmdLineCopy == currentQuote) {
                *cmdLineCopy = '\0';  // Null terminate so we know where it ends
                isInQuoteMode = 0;
                currentQuote = '\0'; // Reset quote for new time
            }
        } else {

            // Check for quotes and if so turn it on
            if (*cmdLineCopy == '"' || *cmdLineCopy == '\'') {
                isInQuoteMode = 1;
                currentQuote = *cmdLineCopy; // Update our opening quote so we know what to look for when we close
                currentArgumentPointer = cmdLineCopy + 1; // Jump over the quote so that we get to the actual argument
            
            // If we have some spaces and we aren't in quotes
            } else if (isspace((unsigned char)*cmdLineCopy)) { 

                // Start new argument, close the previous argument
                if (currentArgumentPointer != NULL) {
                    *cmdLineCopy = '\0';  
                    pointerArray[argumentCount] = currentArgumentPointer;
                    argumentCount++;
                    currentArgumentPointer = NULL;
                }
            } else {
                // So these could be part of an argument, so we continue and we don't break it into another argument
                if (currentArgumentPointer == NULL) {
                    currentArgumentPointer = cmdLineCopy;
                }
            }
        }
        cmdLineCopy++;
    }

    // Make sure that last argument is properly put in array
    if (currentArgumentPointer != NULL) {
        pointerArray[argumentCount] = currentArgumentPointer;
        argumentCount++;
    }

    // Last one gotta be null
    pointerArray[argumentCount] = NULL;

    // Update struct with our pointers
    for (int i = 0; i < argumentCount; i++) {
        cmd_buff->argv[i] = pointerArray[i];
    }

    // Because excevp looks at null to end
    // Also do some updating
    cmd_buff->argv[argumentCount] = NULL;  
    cmd_buff->argc = argumentCount;
    cmd_buff->_cmd_buffer = copyHead; 

    return OK;
}
