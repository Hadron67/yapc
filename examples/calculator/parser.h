#ifndef __YY_H__
#define __YY_H__

#include <stdlib.h>
#include <stdio.h>
#define T_EOF 0
#define T_NUM 1
#define T_PLUS 2
#define T_MINUS 3
#define T_MULTIPLY 4
#define T_DIVIDE 5
#define T_EXP 6
#define T_BRA 7
#define T_KET 8
#define T_SIN 9
#define T_COS 10
#define T_TAN 11
#define T_ASIN 12
#define T_ACOS 13
#define T_ATAN 14
#define T_CEXP 15
#define T_LN 16
#define T_I 17
#define T_PI 18
#define T_E 19


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
    double _Complex *pstack,*sp;
    int pSize;
    //current token,this token would be accepted
    //when yyParser_acceptTokenis called.
    double _Complex token;
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