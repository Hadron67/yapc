#ifndef __YY_H__
#define __YY_H__

#include <stdlib.h>
#include <stdio.h>
#define MT_EOF 0
#define MT_CBRA 1
#define MT_CKET 2
#define MT_MEMBER 3
#define MT_NUM 4
#define MT_STRING 5




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
int jmParser_clearStack(jmParser *jmparser);


#endif