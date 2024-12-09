#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "xsh_env.h"

#define MAX_ENV_VARS 100

typedef struct {
    char key[256];
    char value[256];
} EnvVar;

static EnvVar env_vars[MAX_ENV_VARS];
static int env_count = 0;

void set_env_var(const char *key, const char *value) {
    for (int i = 0; i < env_count; i++) {
        if (strcmp(env_vars[i].key, key) == 0) {
            strncpy(env_vars[i].value, value, sizeof(env_vars[i].value));
            return;
        }
    }

    if (env_count < MAX_ENV_VARS) {
        strncpy(env_vars[env_count].key, key, sizeof(env_vars[env_count].key));
        strncpy(env_vars[env_count].value, value, sizeof(env_vars[env_count].value));
        env_count++;
    } else {
        fprintf(stderr, "Environment variable limit reached.\n");
    }
}

void unset_env_var(const char *key) {
    for (int i = 0; i < env_count; i++) {
        if (strcmp(env_vars[i].key, key) == 0) {
            env_vars[i] = env_vars[env_count - 1]; // Replace with last
            env_count--;
            return;
        }
    }
}

char *get_env_var(const char *key) {
    for (int i = 0; i < env_count; i++) {
        if (strcmp(env_vars[i].key, key) == 0) {
            return env_vars[i].value;
        }
    }
    return NULL;
}

char *replace_env_vars(const char *input) {
    static char result[1024]; // Fixed-size buffer for simplicity
    memset(result, 0, sizeof(result)); // Clear the buffer

    const char *src = input;
    char *dst = result;

    while (*src) {
        if (*src == '$') {
            src++;
            char var_name[128] = {0};
            int var_len = 0;

            // Extract variable name
            while (*src && (isalnum(*src) || *src == '_')) {
                var_name[var_len++] = *src++;
            }
            var_name[var_len] = '\0';

            // Lookup the variable
            const char *value = getenv(var_name);
            if (!value) value = ""; // Default to empty if not found

            // Append the variable value to the result
            while (*value) {
                *dst++ = *value++;
            }
        } else {
            // Copy regular characters
            *dst++ = *src++;
        }
    }

    *dst = '\0'; // Null-terminate the result
    return result;
}

