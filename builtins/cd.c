#include "../shell.h"
#include <unistd.h>

int cd(int argc, char **argv)
{
    chdir(argv[1]);
    return 0;
}