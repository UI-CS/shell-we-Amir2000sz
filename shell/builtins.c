#include "builtins.h"
#include "history.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int handle_builtin(char **args, int *should_run) {
    if (strcmp(args[0], "exit") == 0) {
        *should_run = 0;
        return 1;
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL || chdir(args[1]) != 0)
            perror("cd");
        return 1;
    }

    if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
        return 1;
    }

    if (strcmp(args[0], "help") == 0) {
        printf("Built-ins: exit cd pwd help history\n");
        return 1;
    }

    if (strcmp(args[0], "history") == 0) {
        print_history();
        return 1;
    }

    return 0;
}
