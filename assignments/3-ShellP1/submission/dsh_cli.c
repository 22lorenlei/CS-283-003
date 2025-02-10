#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
int main()
{
    // So this is the max we are gonna have for our command (in length)
    char *cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));

    // This is our return code
    int rc = 0;

    // Our list of commands essentially
    command_list_t clist;

    while (1)
    {
        // So this is essentially printing out dsh>
        printf("%s", SH_PROMPT);

        // Reads until EOF 
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // IMPLEMENT THE REST OF THE REQUIREMENTS

        if (strcmp(cmd_buff, "exit") == 0) {
            rc = OK;
            break;
        } else if (strcmp(cmd_buff, "dragon") == 0) {
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
            break;
        }

        // So if we enter nothing, then we set CMD_WARN_NO_CMD error
        if (strlen(cmd_buff) == 0) {
            rc = WARN_NO_CMDS;
        
        // For the case we have arguments, then we would parse the command string. 
        // We will funnel the output status code from the dshlib.c method to rc
        // Assume nonempty input
        } else {
            rc = build_cmd_list(cmd_buff, &clist);
        }

        // Our return code to messages
        if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
            printf("Command string is too big\n");
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        } else if (rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
        } else if (rc == OK) {
            // Print commands
            printf(CMD_OK_HEADER, clist.num);
            for (int i = 0; i < clist.num; i++) {

                // If we have arguments
                if (strlen(clist.commands[i].args) > 0) {
                    printf("<%d> %s [%s]\n", i + 1, clist.commands[i].exe, clist.commands[i].args);
                    
                } else {
                    printf("<%d> %s\n", i + 1, clist.commands[i].exe);
                }
            }
        }
    }

    free(cmd_buff);

    return OK;
}