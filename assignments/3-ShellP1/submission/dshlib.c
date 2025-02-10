#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
 int build_cmd_list(char *cmd_line, command_list_t *clist)
 {
    int commandCount = 0;

    // Check if it is too long (the command)
    if (strlen(cmd_line) > SH_CMD_MAX) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    // Make a copy of the original command line
    char *cmdLineCopy = strdup(cmd_line);
    if (cmdLineCopy == NULL) {
        return -1;
    }

    // This is the command up to per pipe
    char *commandByPipe;

    // We will store the pointers to start of each command (trimmed)
    char *commands[CMD_MAX];

    // This basically stores the pointers of each command to the nearest pipe
    commandByPipe = strtok(cmdLineCopy, "|");
    while (commandByPipe != NULL) {

        // Check if we have too many commands
        if (commandCount >= CMD_MAX) {
            free(cmdLineCopy);
            return ERR_TOO_MANY_COMMANDS;
        }

        // Trim beginning
        while (*commandByPipe && isspace((unsigned char) *commandByPipe)) {
            commandByPipe++;
        }

        // Make new copies because strtok changes it in place and messes it up
        // Set that pointer (to the beginning of that command) to a list of all beginning pointers of the commands
        commands[commandCount] = strdup(commandByPipe);
        commandCount++;
        commandByPipe = strtok(NULL, "|");
    }

    clist->num = commandCount;

    // This is where we get the arguments and the command tag for each total command
    for (int i = 0; i < commandCount; i++) {

        // This is now segments based on spaces
        char *segment = strtok(commands[i], " ");
        int isCommand = 1;

        // This is the total string that we are going to set to the arg attribute in command list per command
        char arguments[ARG_MAX] = "";
        while (segment != NULL) {

            // Assume that the first segment is the command
            if (isCommand == 1) {
                strcpy(clist->commands[i].exe, segment);
                isCommand = 0;

            // Rest are arguments
            } else {

                // Add onto argument string separated by spaces
                strcat(arguments, segment);
                strcat(arguments, " ");
            }
            segment = strtok(NULL, " ");
        }

        // If we have arguments, then get rid of that last space and replace it with null terminator
        if (strlen(arguments) > 0) {
            arguments[strlen(arguments) - 1] = '\0';
        }

        // Set argument string to the command
        strcpy(clist->commands[i].args, arguments);
        
        free(commands[i]);
    }
    free(cmdLineCopy);

    return OK;
}

 
 
 