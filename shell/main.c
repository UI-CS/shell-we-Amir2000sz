#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#include "parser.h"
#include "builtins.h"
#include "history.h"

#define PROMPT "uinxsh> "

void reap_zombies() {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    char *input;
    char **args;
    int should_run = 1;

    init_history();

    while (should_run) {
        reap_zombies();

        printf(PROMPT);
        fflush(stdout);

        input = read_line();
        if (!input) break;

        if (strcmp(input, "!!") == 0) {
            char *last = get_last_command();
            if (!last) {
                printf("No commands in history\n");
                free(input);
                continue;
            }
            printf("%s\n", last);
            free(input);
            input = strdup(last);
        }

        add_history(input);
        args = parse_input(input);

        if (args[0] == NULL) {
            free_args(args);
            free(input);
            continue;
        }

        if (handle_builtin(args, &should_run)) {
            free_args(args);
            free(input);
            continue;
        }

        execute_command(args);

        free_args(args);
        free(input);
    }

    cleanup_history();
    return 0;
}
