#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "execute_def.h"

typedef enum _yynodetype_t {
    YY_IS_CONST = 1,
    YY_IS_TEMP,
    YY_IS_VAR
}yynodetype_t;

typedef enum _yyvartype_t{
    YY_TYPE_INT = 1,
    YY_TYPE_ANY
}yyvartype_t;

typedef enum _ybop_t{
    YBOP_MINUS = 1,
    YBOP_TIMES,
    YBOP_LT,
    YBOP_GT,
    YBOP_LTOE,
    YBOP_GTOE,
    YBOP_EQ,
    YBOP_NE
}ybop_t;

#include "spool.h"

typedef struct _yynode{
    int id;
    int line;
    int lineCount;
    yynodetype_t type;
    yyvartype_t varType;
    union{
        struct _yyImage{
            ysptr s;
            int len;
        }image;
        int i;    
        double d;   
        int opline;
        int varPos;
        void *ptr;
    }u;
}yynode;


#endif