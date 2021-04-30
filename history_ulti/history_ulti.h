#ifndef __HISTORY_ULTI_H__
#define __HISTORY_ULTI_H__

struct History
{
    char *lines[10];
    int count;
};

struct History histories;

void init_history();
char *copy_Str(char *from, char *to);
void add_to_history(char *commandline);

void print_history();

#endif //