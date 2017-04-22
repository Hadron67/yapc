#ifndef __PARSER_H__
#define __PARSER_H__
#include <setjmp.h>
#include "grammar_builder.h"

typedef enum _ytoken_t{
    T_EOF = 1,
    T_NAME,
    T_STRING,
    T_TOKEN,
    T_TOKEN_DIR,
    T_ARROW,
    T_OR,
    T_EOL,
    T_SEPERATOR,
    T_BLOCK,
    T_TEST_DIR,
    T_EMPTY_DIR,
    T_PROLOGUE,
    T_TYPE_DIR
}ytoken_t;

typedef struct _YToken{
    int line;
    ytoken_t id;
    size_t image;
}YToken;

typedef struct _YGParser{
    FILE *in,*err;
    
    char c;
    char *buf;
    int len,size;
    int line;

    YToken token;

    jmp_buf catcher;
    
    YGBuilder builder;

}YGParser;

int YGParser_init(YGParser *parser,FILE *err);
YGrammar *YGParser_parse(YGParser *parser,FILE *in);

#endif