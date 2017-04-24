#ifndef __YY_H__
#define __YY_H__

#include <stdlib.h>
#include <stdio.h>
#define T_EOF 0
#define T_STRING 1
#define T_NUM 2
#define T_TRUE 3
#define T_FALSE 4
#define T_NULL 5
#define T_UNDEFINED 6
#define T_BRA 7
#define T_KET 8
#define T_CBRA 9
#define T_CKET 10
#define T_BBRA 11
#define T_BKET 12
#define T_COLLON 13
#define T_COMMA 14




typedef struct _yyParser{
    //state stack
    int *state;
    int sLen,sSize;
    //sematic stack
    jsonval *pstack,*sp;
    int pSize;
    //current token,this token would be accepted
    //when yyParser_acceptTokenis called.
    jsonval token;
    //this would be set to 1 when input is accepted.
    int done;
    //this would be set to 1 when a syntax error is detected.
    int error,errToken;
    //the generic pointer that user can set
    void *userData;
}yyParser;


int yyParser_init(yyParser *yyparser);
int yyParser_reInit(yyParser *yyparser);
int yyParser_free(yyParser *yyparser);
int yyParser_acceptToken(yyParser *yyparser,int yytokenid);
int yyParser_printError(yyParser *yyparser,FILE *out);
int yyParser_clearStack(yyParser *yyparser);


#endif