#ifndef __SCANNER_H__
#define __SCANNER_H__

struct token_s
{
    struct source_s *src;
    int text_len;
    char *text;
};

/* the special EOF token, which indicates the end of input */
extern struct token_s eof_token;

struct token_s *tokenize(struct source_s *src);
void free_token(struct token_s *tok);

#endif // __SCANNER_H__