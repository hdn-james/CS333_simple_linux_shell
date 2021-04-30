#ifndef __PARSER_H__
#define __PARSER_H__

#include "scanner.h" /* struct token_s */
#include "source.h"  /* struct source_s */

struct node_s *parse_simple_command(struct token_s *tok);

#endif // __PARSER_H__