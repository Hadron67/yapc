/*
    Generated by yapc -- An LR(1) parser generator made by CFY
*/

#ifndef __YY_H__
#define __YY_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdio.h>
#define YY_OK 0
#define YY_ERR -1
#define YY_SHIFT 1
#define YY_REDUCE 2
#define YY_ACCEPT 3


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
#define T_SINH 12
#define T_COSH 13
#define T_TANH 14
#define T_ASIN 15
#define T_ACOS 16
#define T_ATAN 17
#define T_CEXP 18
#define T_LN 19
#define T_SQRT 20
#define T_I 21
#define T_PI 22
#define T_E 23


extern const char *yytokenNames[];
extern const char *yytokenAlias[];
extern const char *yynonTerminals[];


typedef struct _yyParser{
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


int yyParser_init(yyParser *yyparser);
int yyParser_reInit(yyParser *yyparser);
int yyParser_free(yyParser *yyparser);
int yyParser_acceptToken(yyParser *yyparser,int yytokenid);
int yyParser_printError(yyParser *yyparser,FILE *out);


#ifdef __cpluplus
}
#endif
#endif