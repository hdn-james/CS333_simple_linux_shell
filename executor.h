#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include "node.h"

char *search_path(char *file);
int do_exec_cmd(int argc, char **argv);
int do_simple_command(struct node_s *node);

#endif // __EXECUTOR_H__