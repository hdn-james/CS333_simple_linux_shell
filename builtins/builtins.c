#include "../shell.h"

struct builtin_s builtins[] =
    {
        {"dump", dump},
        {"cd", cd},
        {"history", history},
};

int builtins_count = sizeof(builtins) / sizeof(struct builtin_s);