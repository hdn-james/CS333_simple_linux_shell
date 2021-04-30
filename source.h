#ifndef __SOURCE_H__
#define __SOURCE_H__

#define EOF (-1)
#define ERRCHAR (0)
#define INIT_SRC_POS (-2)

struct source_s
{
    char *buffer;
    long bufsize;
    long curpos;
};

char next_char(struct source_s *src);
void unget_char(struct source_s *src);
char peek_char(struct source_s *src);
void skip_white_space(struct source_s *src);

#endif // __SOURCE_H__