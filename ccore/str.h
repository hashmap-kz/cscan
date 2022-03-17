#ifndef STR_H_
#define STR_H_

#include "hdrs.h"
#include "vec.h"

extern char EMPTY_STRBUF_STR[];
#define STR_INIT  { .buf = EMPTY_STRBUF_STR, .len = 0, .alloc = 0, .offset = 0 }

typedef struct strbuf Str;
struct strbuf {
    char *buf;
    size_t len, alloc, offset;
};

int sb_addc(Str *s, char c);
int sb_adds(Str *s, char *news);
Str *sb_new();
Str *sb_news(char * str);
void sb_reset(Str *s);
char *sb_left(char *from, size_t much);
char *sb_right(char *from, size_t much);
char *sb_mid(char *from, size_t begin, size_t much);
char *sb_trim(char *from);
char *sb_replace(char * input, char * pattern, char * replacement);
char *normalize_slashes(char *s);
int is_abs_win(char *s);
int is_abs_unix(char *s);
int is_abs_path(char *s);
int strstarts(char *what, char *with);
int strends(char *what, char *with);

vec *sb_split_char(char * where, char sep, int include_empty);
char *normalize(char *given);
int strequal(void *a, void *b);
int sb_nextc(Str *buf);
int sb_peekc(Str *buf);

int sb_pop(Str *buf);
int sb_adds_rev(Str *buf, char *input);

#endif /* STR_H_ */
