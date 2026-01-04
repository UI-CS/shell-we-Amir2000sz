#include "parser.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_TOKENS 64

char *read_line() {
    char *line = NULL;
    size_t size = 0;
    getline(&line, &size, stdin);
    line[strcspn(line, "\n")] = 0;
    return line;
}

char **parse_input(char *input) {
    char **tokens = malloc(MAX_TOKENS * sizeof(char *));
    char *token;
    int i = 0;

    token = strtok(input, " ");
    while (token != NULL) {
        tokens[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;
    return tokens;
}

void free_args(char **args) {
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}

void execute_command(char **args) {
    int background = 0;
    int i;

    for (i = 0; args[i]; i++) {
        if (strcmp(args[i], "&") == 0) {
            background = 1;
            args[i] = NULL;
        }
    }

    int pipe_index = -1;
    for (i = 0; args[i]; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_index = i;
            break;
        }
    }

    if (pipe_index != -1) {
        int fd[2];
        pipe(fd);

        pid_t p1 = fork();
        if (p1 == 0) {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            args[pipe_index] = NULL;
            execvp(args[0], args);
            perror("exec");
            exit(1);
        }

        pid_t p2 = fork();
        if (p2 == 0) {
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);
            execvp(args[pipe_index + 1], &args[pipe_index + 1]);
            perror("exec");
            exit(1);
        }

        close(fd[0]);
        close(fd[1]);
        waitpid(p1, NULL, 0);
        waitpid(p2, NULL, 0);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("exec");
        exit(1);
    } else if (!background) {
        waitpid(pid, NULL, 0);
    }
}
