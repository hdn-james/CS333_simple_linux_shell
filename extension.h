#ifndef __EXTENSION_H__
#define __EXTENSION_H__

#include <unistd.h>   // fork()
#include <sys/wait.h> // waitpid()
#include <stdio.h>    // printf(), fgets()
#include <string.h>   // strtok(), strcmp(), strdup()
#include <stdlib.h>   // free()
#include <fcntl.h>    // open(), creat(), close()

// const unsigned BUF_SIZE = 50;
// const unsigned REDIR_SIZE = 2;
// const unsigned PIPE_SIZE = 3;

void parse_command(char input[], char *argv[], int *wait);
void parse_redir(char *argv[], char *redir_argv[]);
int parse_pipe(char *argv[], char *child01_argv[], char *child02_argv[]);
void child(char *argv[], char *redir_argv[]);
void parent(pid_t child_pid, int wait);
void exec_with_pipe(char *child01_argv[], char *child02_argv[]);
int run(char *cmd);

#endif