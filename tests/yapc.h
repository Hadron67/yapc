#ifndef __YY_H__
#define __YY_H__

#include <stdlib.h>
#include <stdio.h>
#define T_EOF 0
#define T_TOKEN_DIR 1
#define T_TYPE_DIR 2
#define T_DATATYPE_DIR 3
#define T_TEST_DIR 4
#define T_EMPTY_DIR 5
#define T_SEPERATOR 6
#define T_ARROW 7
#define T_OR 8
#define T_EOL 9
#define T_NAME 10
#define T_TOKEN 11
#define T_STRING 12
#define T_BLOCK 13
#define T_PROLOGUE 14


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
    size_t *pstack,*sp;
    int pSize;
    //current token,this token would be accepted
    //when yyParser_acceptTokenis called.
    size_t token;
    //this would be set to 1 when input is accepted.
    int done;
    //this would be set to 1 when a syntax error is detected.
    int error,errToken;
    //the generic pointer that user can set
    void *userData;
}yyParser;


int yyParser_init(yyParser *yyparser,yyalloc altor,yyrealloc rtor,yyfree dtor);
int yyParser_free(yyParser *yyparser);
int yyParser_acceptToken(yyParser *yyparser,int yytokenid);
int yyParser_printError(yyParser *yyparser,FILE *out);


#endif