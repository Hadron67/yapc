#ifndef __JSON_MATCH_H__
#define __JSON_MATCH_H__
#include "json.h"

typedef struct _jnode{
    char *sv;
    int nv;
}jnode;

#include "matcher.h"
typedef struct _jMatch{
    jmParser parser;
    jsonval *val;
    FILE *errout;
    int err;
    
    char *buf;
    int len,size;
}jMatch;

int jMatch_init(jMatch *m,FILE *err);
int jMatch_free(jMatch *m);
int jMatch_match(jMatch *m,jsonval *val,const char *input);

int jMatch_doNum(jMatch *m,const jnode *n);
int jMatch_doString(jMatch *m,const jnode *n);
int jMatch_doMember(jMatch *m,const jnode *n);

#endif
