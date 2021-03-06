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
#define T_TOKEN_DIR 1
#define T_TYPE_DIR 2
#define T_DATATYPE_DIR 3
#define T_TEST_DIR 4
#define T_EMPTY_DIR 5
#define T_TOKEN_PREFIX_DIR 6
#define T_ENABLE_CST_DIR 7
#define T_LEFT_DIR 8
#define T_RIGHT_DIR 9
#define T_NONASSOC_DIR 10
#define T_PREC_DIR 11
#define T_NS_DIR 12
#define T_SEPERATOR 13
#define T_ARROW 14
#define T_OR 15
#define T_EOL 16
#define T_NEG 17
#define T_NAME 18
#define T_NUM 19
#define T_TOKEN 20
#define T_STRING 21
#define T_BLOCK 22
#define T_PROLOGUE 23


extern const char *yytokenNames[];
extern const char *yytokenAlias[];
extern const char *yynonTerminals[];


typedef struct _yyParser{
    //state stack
    int *state;
    int sLen,sSize;
    //sematic stack
    YToken *pstack,*sp;
    int pSize;
    //current token,this token would be accepted
    //when yyParser_acceptTokenis called.
    YToken token;
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