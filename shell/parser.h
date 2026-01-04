#ifndef PARSER_H
#define PARSER_H

char *read_line(void);
char **parse_input(char *input);
void free_args(char **args);
void execute_command(char **args);

#endif
