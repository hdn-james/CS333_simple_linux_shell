#include "../shell.h"
#include "../symbol_table/symbol_table.h"
#include <unistd.h>

int cd(int argc, char **argv)
{
    chdir(argv[1]);
    char cwd[1024];
    struct symtab_entry_s *entry_PWD = get_symtab_entry("CURR_PWD");

    getcwd(cwd, 1024);
    symtab_entry_setval(entry_PWD, cwd);
    return 0;
}
