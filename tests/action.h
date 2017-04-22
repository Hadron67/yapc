#ifndef __YY_H__
#define __YY_H__

#include <stdlib.h>
#include <stdio.h>
#define T_EOF 0
#define T_ID 1
#define T_PLUS 2
#define T_NUM 3


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