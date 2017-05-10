#include <assert.h>
#include "execute.h"

#define AR ((YAREntry *)YArray_get(&vm->ar,vm->ar.len - 1))

static yvm_status_t exit_h(yvm *vm){
    return YVM_STATUS_EXIT;
}

static yvm_status_t iconst_h(yvm *vm){
    yval *val = (yval *)YArray_push(&vm->stack);
    val->type = YVAL_TYPE_INT;
    val->u.i = vm->pc->arg1.i;

    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t load_h(yvm *vm){
    yval *val = (yval *)YArray_get(&vm->stack,AR->vsp + vm->pc->arg1.i);
    *(yval *)YArray_push(&vm->stack) = *val;

    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t store_h(yvm *vm){
    yval *val = (yval *)YArray_pop(&vm->stack);
    *(yval *)YArray_get(&vm->stack,AR->vsp + vm->pc->arg1.i) = *val;

    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t push_h(yvm *vm){
    YArray_push(&vm->stack);

    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t pop_h(yvm *vm){
    YArray_pop(&vm->stack);
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t plus_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i + op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}


static yvm_status_t minus_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i - op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t times_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i * op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t divide_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i / op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t mod_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i % op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t lt_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i < op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t gt_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i > op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t ltoe_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i <= op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t gtoe_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i >= op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t eq_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i == op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t ne_h(yvm *vm){
    yval *op2 = (yval *)YArray_pop(&vm->stack);
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    yval op;
    op.type = op1->type;
    op.u.i = op1->u.i != op2->u.i;
    *(yval *)YArray_push(&vm->stack) = op;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t invoke_h(yvm *vm){
    YAREntry *ar = (YAREntry *)YArray_push(&vm->ar);
    ar->vsp = vm->stack.len - 1;
    ar->ret = vm->pc - vm->op + 1;
    vm->pc = vm->op + vm->pc->arg1.i;
    return YVM_STATUS_OK;
}

static yvm_status_t ret_h(yvm *vm){
    YAREntry *ar = (YAREntry *)YArray_pop(&vm->ar);
    vm->pc = vm->op + ar->ret;
    return YVM_STATUS_OK;
}

static yvm_status_t subvsp_h(yvm *vm){
    YAREntry *ar = (YAREntry *)YArray_get(&vm->ar,vm->ar.len - 1);
    ar->vsp -= vm->pc->arg1.i;
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t jmp_h(yvm *vm){
    vm->pc = vm->op + vm->pc->arg1.i;
    return YVM_STATUS_OK;
}

static yvm_status_t jmp_z_h(yvm *vm){
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    if(op1->u.i == 0){
        vm->pc = vm->op + vm->pc->arg1.i;
    }
    else {
        vm->pc++;
    }
    return YVM_STATUS_OK;
}

static yvm_status_t jmp_nz_h(yvm *vm){
    yval *op1 = (yval *)YArray_pop(&vm->stack);
    if(op1->u.i != 0){
        vm->pc = vm->op + vm->pc->arg1.i;
    }
    else {
        vm->pc++;
    }
    return YVM_STATUS_OK;
}

static yvm_status_t read_h(yvm *vm){
    yval *val = (yval *)YArray_push(&vm->stack);
    val->type = YVAL_TYPE_INT;
    fscanf(vm->in,"%d",&val->u.i);
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_status_t write_h(yvm *vm){
    yval *val = (yval *)YArray_pop(&vm->stack);
    fprintf(vm->out,"%d\n",val->u.i);
    vm->pc++;
    return YVM_STATUS_OK;
}

static yvm_handler default_handlers[] = {
    exit_h,
    iconst_h,
    load_h,
    store_h,
    push_h,
    pop_h,

    plus_h,
    minus_h,
    times_h,
    divide_h,
    mod_h,
    lt_h,
    gt_h,
    ltoe_h,
    gtoe_h,
    eq_h,
    ne_h,

    invoke_h,
    ret_h,
    subvsp_h,
    jmp_h,
    jmp_z_h,
    jmp_nz_h,

    read_h,
    write_h

};

int yvm_init(yvm *vm,FILE *in,FILE *out){
    YArray_init(&vm->stack,sizeof(yval),32);
    YArray_init(&vm->ar,sizeof(YAREntry),8);

    vm->in = in;
    vm->out = out;
    vm->handlers = default_handlers;

    vm->recursionLimit = 64;

    return 0;
}
int yvm_free(yvm *vm){
    YArray_free(&vm->stack,NULL);
    YArray_free(&vm->ar,NULL);
    return 0;
}
int yvm_loadProg(yvm *vm,yop *op){
    vm->pc = vm->op = op;
    vm->stack.len = vm->ar.len = 0;
    YAREntry *ar = (YAREntry *)YArray_push(&vm->ar);
    ar->vsp = 0;
    ar->ret = 0;
    return 0;
}
yvm_status_t yvm_executeOne(yvm *vm){
    //yop_dump(vm->pc,vm->pc - vm->op,vm->out);
    return vm->handlers[vm->pc->type - 1](vm);
}
static yvm_status_t yvm_checkErr(yvm *vm){
    if(vm->ar.len >= vm->recursionLimit){
        fprintf(vm->out,"fatal error: maximum recursion depth exceeded\n");
        return YVM_STATUS_ERR;
    }
    return YVM_STATUS_OK;
}
yvm_status_t yvm_execute(yvm *vm){
    yyexe:
    if(yvm_checkErr(vm) != YVM_STATUS_OK){
        return YVM_STATUS_ERR;
    }
    switch(yvm_executeOne(vm)){
        case YVM_STATUS_OK:
            goto yyexe;
        case YVM_STATUS_EXIT:
            return YVM_STATUS_OK;
        case YVM_STATUS_ERR:
            return YVM_STATUS_ERR;
        default:
            assert(0);
    }
}
static const char *const opNames[] = {
    "(bad op)",

    "exit",

    "iconst",
    "load",
    "store",
    "push",
    "pop",

    "plus",
    "minus",
    "times",
    "divide",
    "mod",
    "lt",
    "gt",
    "ltoe",
    "gtoe",
    "eq",
    "neq",

    "invoke",
    "ret",
    "subvsp",
    "jmp",
    "jmp_z",
    "jmp_nz",

    "read",
    "write"
};
int yop_dump(const yop *op,int i,FILE *out){
    fprintf(out,"%-4d %-8s",i,opNames[op->type]);
    switch(op->type){
        case YOP_ICONST:
        case YOP_JMP:
        case YOP_JMP_Z:
        case YOP_JMP_NZ:
            fprintf(out,"%-5d",op->arg1.i);    
            break;    
        case YOP_LOAD:
        case YOP_STORE:
        case YOP_SUBVSP:
        case YOP_INVOKE:
            fprintf(out,"$%-5d",op->arg1.i);    
            break; 
        default:;    
    }
    fprintf(out,"\n");
    return 0;
}
int yoparray_dump(const yop *array,int len,FILE *out){
    int i;
    for(i = 0;i < len;i++){
        const yop *op = array + i;
        yop_dump(op,i,out);
    }
}