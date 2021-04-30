#include <stdio.h>
#include <unistd.h>

#include "shell.h"
#include "symbol_table/symbol_table.h"

void print_prompt1(void)
{
    struct symtab_entry_s *entry = get_symtab_entry("PS1");
    char cwd[1024];
    getcwd(cwd, 1024);
    if (entry && entry->val)
        fprintf(stderr, "\n%s%s> ", entry->val, cwd);
    else
        fprintf(stderr, "\n$ %s> ", cwd);
}

void print_prompt2(void)
{
    struct symtab_entry_s *entry = get_symtab_entry("PS2");

    if (entry && entry->val)
        fprintf(stderr, "%s", entry->val);
    else
        fprintf(stderr, "> ");
}
