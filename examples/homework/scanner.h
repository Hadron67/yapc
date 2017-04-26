#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdio.h>
#include "parser.h"

extern const char *tokenNames[];

typedef struct _yyStringBlock{
    struct _yyStringBlock *next;
    char data[1];
}yyStringBlock;

typedef struct _yyToken{
    int id;
    int line;
    union{
        char *image;
        int i;        
    }u;
}yyToken;

typedef struct _yyScanner{
    FILE *in,*err;
    int line;
    char c;
    int first;
    
    yyStringBlock *head;
    
    char *buf;
    int len,size;
}yyScanner;

int yyScanner_init(yyScanner *s,FILE *in,FILE *err);
int yyScanner_free(yyScanner *s);
int yyScanner_next(yyScanner *s,yyToken *t);


#endif
