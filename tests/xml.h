#ifndef __YY_H__
#define __YY_H__

#include <stdlib.h>
#include <stdio.h>
#define T_EOF 0
#define T_GT 1
#define T_LT 2
#define T_EQU 3
#define T_COLLON 4
#define T_LT_ASK 5
#define T_GL_ASK 6
#define T_SLASH 7
#define T_ID 8
#define T_STRING 9
#define T_CONTENT 10




typedef struct _yyParser{
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


int yyParser_init(yyParser *yyparser);
int yyParser_reInit(yyParser *yyparser);
int yyParser_free(yyParser *yyparser);
int yyParser_acceptToken(yyParser *yyparser,int yytokenid);
int yyParser_printError(yyParser *yyparser,FILE *out);
int yyParser_clearStack(yyParser *yyparser);


#endif