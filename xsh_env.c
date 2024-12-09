#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "xsh_env.h"

#define MAX_ENV_VARS 100
#define MAX_VAR_NAME 128
#define MAX_VAR_VALUE 256

typedef struct
{
    char name[MAX_VAR_NAME];
    char value[MAX_VAR_VALUE];
} EnvVar;

static EnvVar env_vars[MAX_ENV_VARS];
static int env_count = 0;

void set_env_var(const char *key, const char *value)
{
    for (int i = 0; i < env_count; i++)
    {
        if (strcmp(env_vars[i].name, key) == 0)
        {
            // Update existing variable
            strncpy(env_vars[i].value, value, MAX_VAR_VALUE);
            return;
        }
    }

    // Add a new variable if space permits
    if (env_count < MAX_ENV_VARS)
    {
        strncpy(env_vars[env_count].name, key, MAX_VAR_NAME);
        strncpy(env_vars[env_count].value, value, MAX_VAR_VALUE);
        env_count++;
    }
    else
    {
        printf("Environment variable limit reached.\n");
    }
}

void unset_env_var(const char *key)
{
    for (int i = 0; i < env_count; i++)
    {
        if (strcmp(env_vars[i].name, key) == 0)
        {
            // Shift remaining variables to fill the gap
            for (int j = i; j < env_count - 1; j++)
            {
                env_vars[j] = env_vars[j + 1];
            }
            env_count--;
            return;
        }
    }
    printf("Variable %s not found.\n", key);
}

char *get_env_var(const char *name)
{
    for (int i = 0; i < env_count; i++)
    {
        if (strcmp(env_vars[i].name, name) == 0)
        {
            return env_vars[i].value;
        }
    }
    return NULL;
}

char *replace_env_vars(const char *input)
{
    static char result[1024];
    memset(result, 0, sizeof(result));

    const char *src = input;
    char *dst = result;

    while (*src)
    {
        if (*src == '$')
        {
            src++;
            char var_name[MAX_VAR_NAME] = {0};
            int var_len = 0;

            // Extract variable name
            while (*src && (isalnum(*src) || *src == '_'))
            {
                var_name[var_len++] = *src++;
            }
            var_name[var_len] = '\0';

            // Lookup the variable in the custom environment map
            const char *value = NULL;
            for (int i = 0; i < env_count; i++)
            {
                if (strcmp(env_vars[i].name, var_name) == 0)
                {
                    value = env_vars[i].value;
                    break;
                }
            }
            if (!value)
                value = ""; // Default to empty if not found

            // Append the variable value to the result
            while (*value)
            {
                *dst++ = *value++;
            }
        }
        else
        {
            // Copy regular characters
            *dst++ = *src++;
        }
    }

    *dst = '\0'; // Null-terminate the result
    return result;
}
