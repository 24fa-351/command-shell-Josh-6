#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "xsh_commands.h"

void handle_cd(const char *path)
{
    if (chdir(path) != 0)
    {
        perror("cd");
    }
}

void handle_pwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd");
    }
}
