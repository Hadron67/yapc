#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "parser.h"

#define YTYPEID_REAL   1
#define YTYPEID_CHAR   2
#define YTYPEID_STRING 3


typedef struct _yyToken{
    int id;
    int line;
    int lineCount;
    union{
        struct _yyTokenImage{
            char *s;
            int len;
        }image;
        int i;    
        int typeid;
        double d;    
    }u;
}yyToken;


#endif