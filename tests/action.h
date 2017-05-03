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
#define T_ID 1
#define T_PLUS 2
#define T_NUM 3


extern const char *yytokenNames[];
extern const char *yytokenAlias[];
extern const char *yynonTerminals[];


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


#ifdef __cpluplus
}
#endif
#endif