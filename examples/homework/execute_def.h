#ifndef __YYEXECUTE_DEF_H__
#define __YYEXECUTE_DEF_H__

#include <stdio.h>

typedef enum _yop_t{
    // one arg,exit status
    YOP_EXIT = 1,

    // data transitions
    YOP_ICONST,
    YOP_LOAD,
    YOP_STORE,
    YOP_PUSH,
    YOP_POP,

    // operators
    YOP_PLUS,
    YOP_MINUS,
    YOP_TIMES,
    YOP_DIVIDE,
    YOP_MOD,
    YOP_LT,
    YOP_GT,
    YOP_LTOE,
    YOP_GTOE,
    YOP_EQ,
    YOP_NEQ,

    // controls
    YOP_INVOKE,
    YOP_RET,
    YOP_SUBVSP,
    YOP_JMP,
    YOP_JMP_Z,
    YOP_JMP_NZ,

    YOP_READ,
    YOP_WRITE

}yop_t;

typedef struct _yop{
    yop_t type;
    union _yop_arg1{
        int i;
    } arg1;
}yop;

typedef unsigned int yopline_t;

typedef enum _yval_type_t{
    YVAL_TYPE_INT = 1,
}yval_type_t;

typedef struct _yval{
    yval_type_t type;
    union {
        int i;
        double d;
    }u;
}yval;

int yop_dump(const yop *op,int i,FILE *out);
int yoparray_dump(const yop *array,int len,FILE *out);

#endif