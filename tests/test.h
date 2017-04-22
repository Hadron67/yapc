#ifndef __YY_H__
#define __YY_H__

#include <stdlib.h>
#include <stdio.h>
#define T_EOF 0
#define T_PLUS 1
#define T_MINUS 2
#define T_DIVIDE 3
#define T_TIMES 4
#define T_BRA 5
#define T_KET 6
#define T_ID 7
#define T_NUM 8
#define T_COMMA 9
#define T_ASK 10
#define T_COLLON 11
#define T_SEMI_COLLON 12
#define T_EQU 13
#define T_PLUS_A 14
#define T_MINUS_A 15
#define T_TIMES_A 16
#define T_DIVIDE_A 17
#define T_NOT 18
#define T_CBRA 19
#define T_CKET 20
#define T_CURLY_BRA 21
#define T_CURLY_KET 22
#define T_ARROW 23
#define T_IF 24
#define T_ELSE 25
#define T_WHILE 26
#define T_FOR 27
#define T_FUNCTION 28
#define T_MEMBER 29


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