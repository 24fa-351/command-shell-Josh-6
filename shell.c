#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "xsh_env.h"
#include "xsh_utils.h"
#include "xsh_commands.h"

#define MAX_INPUT 1024

void main_loop() {
    char input[MAX_INPUT];
    while (1) {
        printf("xsh# ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break; // EOF or Ctrl+D
        }

        input[strcspn(input, "\n")] = 0; // Remove trailing newline
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            break;
        }

        handle_input(input);
    }
}

int main() {
    printf("Welcome to xsh shell! Type 'exit' or 'quit' to leave.\n");
    main_loop();
    return 0;
}

