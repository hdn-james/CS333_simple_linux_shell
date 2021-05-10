#include <stdio.h>
#include <unistd.h>

#include "shell.h"
#include "symbol_table/symbol_table.h"

void print_prompt1(void)
{
    struct symtab_entry_s *entry = get_symtab_entry("PS1");
    struct symtab_entry_s *entry_PWD = get_symtab_entry("CURR_PWD");
    if (entry && entry->val)
    {
        fprintf(stderr, "\n%s%s> ", entry->val, entry_PWD->val);
    }

    else
        fprintf(stderr, "\nosh> %s> ", entry_PWD->val);
}

void print_prompt2(void)
{
    struct symtab_entry_s *entry = get_symtab_entry("PS2");

    if (entry && entry->val)
        fprintf(stderr, "%s", entry->val);
    else
        fprintf(stderr, "> ");
}
