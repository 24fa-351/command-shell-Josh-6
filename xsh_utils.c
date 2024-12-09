#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "xsh_env.h"
#include "xsh_commands.h"

void execute_command(char *command, char *args[], int background)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        if (execvp(command, args) == -1)
        {
            perror("xsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("xsh");
    }
    else
    {
        if (!background)
        {
            waitpid(pid, NULL, 0);
        }
    }
}

void handle_input(const char *input)
{
    char buffer[1024];
    strncpy(buffer, replace_env_vars(input), sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char *args[100];
    char *token = strtok(buffer, " ");
    int background = 0;

    if (!token)
        return;

    int arg_count = 0;
    while (token)
    {
        if (strcmp(token, "&") == 0)
        {
            background = 1;
            break;
        }
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    if (strcmp(args[0], "cd") == 0)
    {
        handle_cd(args[1] ? args[1] : getenv("HOME"));
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        handle_pwd();
    }
    else if (strcmp(args[0], "set") == 0)
    {
        if (args[1] && args[2])
        {
            set_env_var(args[1], args[2]);
        }
        else
        {
            printf("Usage: set <key> <value>\n");
        }
    }
    else if (strcmp(args[0], "unset") == 0)
    {
        if (args[1])
        {
            unset_env_var(args[1]);
        }
        else
        {
            printf("Usage: unset <key>\n");
        }
    }
    else if (strcmp(args[0], "echo") == 0)
    {
        for (int i = 1; i < arg_count; i++)
        {
            printf("%s ", args[i]);
        }
        printf("\n");
    }
    else
    {
        execute_command(args[0], args, background);
    }
}
