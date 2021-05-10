#include <string.h>
#include <unistd.h>

#include "shell.h"
#include "symbol_table/symbol_table.h"

extern char **environ;

void initshell(void)
{
    init_symtab();

    struct symtab_entry_s *entry;
    char **p2 = environ;

    while (*p2)
    {
        char *eq = strchr(*p2, '=');
        if (eq)
        {
            int len = eq - (*p2);
            char name[len + 1];

            strncpy(name, *p2, len);
            name[len] = '\0';
            entry = add_to_symtab(name);

            if (entry)
            {
                symtab_entry_setval(entry, eq + 1);
                entry->flags |= FLAG_EXPORT;
            }
        }
        else
            entry = add_to_symtab(*p2);
        p2++;
    }
    entry = add_to_symtab("PS1");
    symtab_entry_setval(entry, "osh> ");

    entry = add_to_symtab("PS2");
    symtab_entry_setval(entry, "> ");

    entry = add_to_symtab("CURR_PWD");
    char cwd[1024];
    getcwd(cwd, 1024);
    symtab_entry_setval(entry, cwd);
}