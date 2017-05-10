#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdio.h>
#include "token.h"
#include "ydef.h"

extern const char *tokenNames[];

typedef struct _yyScanner{
    FILE *in,*err;
    int line;
    char c;
    int first;
    
    YSPool pool;
    
    char *buf;
    int len,size;
}yyScanner;

int yyScanner_init(yyScanner *s,FILE *in,FILE *err);
int yyScanner_free(yyScanner *s,char **pool);
int yyScanner_next(yyScanner *s,yynode *t);


#endif
