#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "shell.h"
#include "source.h"
#include "parser.h"
#include "executor.h"
#include "history_util/history_util.h"
#include "extension.h"

void mySignal(int signo)
{
    if (signo == SIGTERM)
    {
        printf("\nProcess %d SIGTERM detected.\n", getpid());
    }
    if (signo == SIGINT)
    {
        printf("\nProcess %d SIGINT detected.\n", getpid());
    }
    if (signo == SIGTSTP)
    {
        printf("\nProcess %d SIGTSTP detected.\n", getpid());
    }
    /* SIGCHLD needs some support for terminated process */
    if (signo == SIGCHLD)
    {
        int saved_errno;
        saved_errno = errno;
        while (waitpid((pid_t)-1, 0, WNOHANG) > 0)
        {
        }
        errno = saved_errno;
        printf("\nProcess %d received SIGCHLD.\n", getpid());
    }
}

int main(int argc, char **argv)
{
    char *cmd;
    char *temp = malloc(1025);
    int isOnHis = 0;

    initshell();
    init_history();
    signal(SIGTERM, mySignal);
    signal(SIGINT, mySignal);
    signal(SIGCHLD, mySignal);
    signal(SIGTSTP, mySignal);
    do
    {
        print_prompt1();
        if (isOnHis == 0)
            cmd = read_cmd();
        else
        {
            char c;
            isOnHis = 0;
            strcpy(cmd, temp);
            do
            {
                scanf("%c", &c);
            } while (c != 10);
        }
        add_to_history(cmd);
        if (!cmd)
        {
            exit(EXIT_SUCCESS);
        }
        if (cmd[0] == '\0' || strcmp(cmd, "\n") == 0)
        {
            free(cmd);
            continue;
        }
        if (strcmp(cmd, "exit\n") == 0)
        {
            free(cmd);
            break;
        }
        if (strcmp(cmd, "!!\n") == 0)
        {
            isOnHis = 1;
            if (histories.count == 0)
            {
                fprintf(stderr, "No commands in history\n");
                continue;
            }

            strcpy(temp, histories.lines[histories.count - 2]);
            temp[strcspn(temp, "\n")] = '\0';
            printf("%s", temp);
            continue;
        }
        if (strchr(cmd, '<') || strchr(cmd, '>') || strchr(cmd, '|') || cmd[strlen(cmd) - 2] == '&')
        {
            run(cmd);
            free(cmd);
            continue;
        }
        signal(SIGINT, mySignal);
        signal(SIGTERM, mySignal);
        signal(SIGCHLD, mySignal);
        signal(SIGTSTP, mySignal);
        struct source_s src;
        src.buffer = cmd;
        src.bufsize = strlen(cmd);
        src.curpos = INIT_SRC_POS;
        parse_and_execute(&src);
        free(cmd);
    } while (1);
    exit(EXIT_SUCCESS);
}

char *read_cmd(void)
{
    char buf[1024];
    char *ptr = NULL;
    char ptrlen = 0;

    while (fgets(buf, 1024, stdin))
    {
        int buflen = strlen(buf);
        if (!ptr)
            ptr = malloc(buflen + 1);
        else
        {
            char *ptr2 = realloc(ptr, ptrlen + buflen + 1);
            if (ptr2)
                ptr = ptr2;
            else
            {
                free(ptr);
                ptr = NULL;
            }
        }
        if (!ptr)
        {
            fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
            return NULL;
        }
        strcpy(ptr + ptrlen, buf);
        if (buf[buflen - 1] == '\n')
        {
            if (buflen == 1 || buf[buflen - 2] != '\\')
                return ptr;
            ptr[ptrlen + buflen - 2] = '\0';
            buflen -= 2;
            print_prompt2();
        }
        ptrlen += buflen;
    }
    return ptr;
}

int parse_and_execute(struct source_s *src)
{
    skip_white_space(src);

    struct token_s *tok = tokenize(src);

    if (tok == &eof_token)
        return 0;

    while (tok && tok != &eof_token)
    {
        struct node_s *cmd = parse_simple_command(tok);

        if (!cmd)
            break;

        do_simple_command(cmd);
        free_node_tree(cmd);
        tok = tokenize(src);
    }

    return 1;
}