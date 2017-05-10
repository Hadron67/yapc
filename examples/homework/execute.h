#ifndef __EXECUTE_H__
#define __EXECUTE_H__

#include "execute_def.h"
#include "dynamic_array.h"

typedef struct _yvm yvm;

typedef enum yvm_status_t{
    YVM_STATUS_OK = 1,
    YVM_STATUS_ERR,
    YVM_STATUS_EXIT,
}yvm_status_t;

typedef yvm_status_t (*yvm_handler)(yvm *vm);


typedef struct _YAREntry{
    unsigned int vsp;
    unsigned int ret;
}YAREntry;

struct _yvm{
    YArray stack;
    YArray ar;
    yop *op;
    yop *pc;
    yvm_handler *handlers;

    int recursionLimit;

    FILE *in,*out;
};


int yvm_init(yvm *vm,FILE *in,FILE *out);
int yvm_free(yvm *vm);
int yvm_loadProg(yvm *vm,yop *op);
yvm_status_t yvm_executeOne(yvm *vm);
yvm_status_t yvm_execute(yvm *vm);


#endif