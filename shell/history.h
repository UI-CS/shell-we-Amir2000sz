#ifndef HISTORY_H
#define HISTORY_H

void init_history(void);
void add_history(const char *cmd);
char *get_last_command(void);
void print_history(void);
void cleanup_history(void);

#endif
