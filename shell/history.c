#include "history.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *last_command = NULL;

void init_history() {}

void add_history(const char *cmd) {
    free(last_command);
    last_command = strdup(cmd);
}

char *get_last_command() {
    return last_command;
}

void print_history() {
    if (last_command)
        printf("1 %s\n", last_command);
}

void cleanup_history() {
    free(last_command);
}
