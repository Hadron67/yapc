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


#define MT_EOF 0
#define MT_CBRA 1
#define MT_CKET 2
#define MT_MEMBER 3
#define MT_NUM 4
#define MT_STRING 5


extern const char *jmtokenNames[];
extern const char *jmtokenAlias[];
extern const char *jmnonTerminals[];


typedef struct _jmParser{
    //state stack
    int *state;
    int sLen,sSize;
    //sematic stack
    jnode *pstack,*sp;
    int pSize;
    //current token,this token would be accepted
    //when jmParser_acceptTokenis called.
    jnode token;
    //this would be set to 1 when input is accepted.
    int done;
    //this would be set to 1 when a syntax error is detected.
    int error,errToken;
    //the generic pointer that user can set
    void *userData;
}jmParser;


int jmParser_init(jmParser *jmparser);
int jmParser_reInit(jmParser *jmparser);
int jmParser_free(jmParser *jmparser);
int jmParser_acceptToken(jmParser *jmparser,int jmtokenid);
int jmParser_printError(jmParser *jmparser,FILE *out);


#ifdef __cpluplus
}
#endif
#endif