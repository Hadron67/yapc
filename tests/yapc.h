#ifndef __YY_H__
#define __YY_H__

#include <stdlib.h>
#include <stdio.h>
#define T_EOF 0
#define T_TOKEN_DIR 1
#define T_TEST_DIR 2
#define T_SEPERATOR 3
#define T_COLLON 4
#define T_OR 5
#define T_SEMI_COLLON 6
#define T_ID 7
#define T_TOKEN 8
#define T_STRING 9
#define T_BLOCK 10


typedef void *(*yyalloc)(size_t size);
typedef void *(*yyrealloc)(void *p,size_t size);
typedef void (*yyfree)(void *p);


typedef struct _yyParser{
    yyalloc altor;
    yyfree dtor;
    yyrealloc rtor;
    //state stack
    int *state;
    int sLen,sSize;
    //sematic stack
    int *pstack,*sp;
    int pSize;
    //current token,this token would be accepted
    //when yyParser_acceptTokenis called.
    int token;
    //this would be set to 1 when a syntax error is detected.
    int error,errToken;
}yyParser;


int yyParser_init(yyParser *yyparser,yyalloc altor,yyrealloc rtor,yyfree dtor);
int yyParser_free(yyParser *yyparser);
int yyParser_acceptToken(yyParser *yyparser,int yytokenid);
int yyParser_printError(yyParser *yyparser,FILE *out);


#endif