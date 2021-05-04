#include "../shell.h"
#include "../history_util/history_util.h"

int history(int argc, char **argv)
{
    print_history();
    return 0;
}