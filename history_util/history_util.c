#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "history_util.h"

void init_history()
{
    int i;
    for (i = 0; i < 10; i++)
    {
        histories.lines[i] = NULL;
    }
    histories.count = 0;
}

char *copy_Str(char *from, char *to)
{
    for (char *p = to; (*p = *from) != '\0'; ++p, ++from)
    {
        ;
    }

    return to;
}

void add_to_history(char *commandline)
{
    int i;
    if (histories.count < 10)
    {
        histories.lines[histories.count] = malloc(strlen(commandline) + 1);
        histories.lines[histories.count] = copy_Str(commandline, histories.lines[histories.count]);
        histories.count += 1;
    }
    else
    {
        for (i = 0; i < 9; ++i)
        {
            free(histories.lines[i]);
            histories.lines[i] = malloc(strlen(histories.lines[i + 1]) + 1);
            histories.lines[i] = copy_Str(histories.lines[i + 1], histories.lines[i]);
        }
        histories.lines[histories.count - 1] = malloc(strlen(commandline) + 1);
        histories.lines[histories.count - 1] = copy_Str(commandline, histories.lines[histories.count - 1]);
    }
}

void print_history()
{
    int i;
    for (i = 0; i < histories.count; i++)
    {
        printf("%s", histories.lines[i]);
    }
}
