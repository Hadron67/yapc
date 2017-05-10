#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "ydef.h"
#include "compile.h"

#define OP(line) (yyCompiler_getOp(c,(line)))
#define EMITE() yyCompiler_emit(c)
#define STR(s) (YSPool_getString(c->pool,(s)))

#define VARPOS(v) (v)

//==========================================================================
typedef struct _yfuncCall{
    int isMarker;
    int line;
    int argCount;
    yopline_t opline;
    ysptr fname;
}yfuncCall;

static yop *yyCompiler_getOp(yyCompiler *c,yopline_t line){
    return c->oparray + line;
}

static int yyCompiler_vaerr(yyCompiler *s,int line,const char *fmt,va_list args){
    if(s->err != NULL){
        fprintf(s->err," *** compilation error: %d:",line);
        vfprintf(s->err,fmt,args);
        fprintf(s->err,"\n");
    }
    s->status = -1;
    return 0;
}

static int yyCompiler_err(yyCompiler *s,int line,const char *fmt,...){
    va_list args;
    va_start(args,fmt);
    yyCompiler_vaerr(s,line,fmt,args);
    va_end(args);
    return 0;
}
 
int yyCompiler_init(yyCompiler *c,FILE *err,YSPool *pool){
    memset(c,0,sizeof(yyCompiler));
    YArray_init(&c->vars,sizeof(YVarEntry),16);
    YArray_init(&c->funcs,sizeof(YFuncEntry),16);
    YArray_init(&c->funcCalls,sizeof(yfuncCall),16);
    c->err = err;
    c->scopeLevel = 0;

    c->opLen = 0;
    c->opSize = 32;
    c->oparray = (yop *)ya_malloc(NULL,sizeof(yop) * c->opSize);

    c->pool = pool;
    return 0;
}
int yyCompiler_free(yyCompiler *c){
    YArray_free(&c->vars,NULL);
    YArray_free(&c->funcs,NULL);
    YArray_free(&c->funcCalls,NULL);
    ya_free(NULL,c->oparray);
    return 0;
}

static yopline_t yyCompiler_emit(yyCompiler *c){
    if(c->opLen >= c->opSize){
        c->opSize *= 2;
        c->oparray = (yop *)ya_realloc(NULL,c->oparray,sizeof(yop) * c->opSize);
    }
    return c->opLen++;
}
static YVarEntry *yyCompiler_findVar(yyCompiler *c,ysptr name,int inScope){
    int i = c->vars.len;
    const char *vname = STR(name);
    while(i --> 0){
        YVarEntry *var = (YVarEntry *)YArray_get(&c->vars,i);
        if(var->entryType != YVARENTRY_MARKER){
            if(!strcmp(vname,STR(var->name))){
                return var;
            }
        }
        else if(var->entryType == YVARENTRY_MARKER && inScope){
            break;
        }
    }
    return NULL;
}
static int yyCompiler_getVarCount(yyCompiler *c){
    int ret = 0,i = c->vars.len - 1;
    YVarEntry *sym = (YVarEntry *)YArray_get(&c->vars,i--);
    while(sym->entryType != YVARENTRY_MARKER){
        ret++;
        sym = (YVarEntry *)YArray_get(&c->vars,i--);
    }
    return ret;
}
static YFuncEntry *yyCompiler_findFunc(yyCompiler *c,ysptr name,int inScope){
    int i = c->funcs.len;
    const char *fname = STR(name);
    while(i --> 0){
        YFuncEntry *func = (YFuncEntry *)YArray_get(&c->funcs,i);
        if(!func->isMarker){
            if(!strcmp(fname,STR(func->name))){
                return func;
            }
        }
        else if(inScope){
            break;
        }
    }
    return NULL;
}

int yyCompiler_doEnterScope(yyCompiler *c){
    YVarEntry *var = (YVarEntry *)YArray_push(&c->vars);
    var->entryType = YVARENTRY_MARKER;

    YFuncEntry *func = (YFuncEntry *)YArray_push(&c->funcs);
    func->isMarker = 1;

    yfuncCall *call = (yfuncCall *)YArray_push(&c->funcCalls);
    call->isMarker = 1;

    c->scopeLevel++;
    return 0;
}
int yyCompiler_doLeaveScope(yyCompiler *c){
    c->scopeLevel--;
    YVarEntry *var = (YVarEntry *)YArray_pop(&c->vars);
    while(var->entryType != YVARENTRY_MARKER){
        if(var->entryType == YVARENTRY_VAR || var->entryType == YVARENTRY_ARG){
            OP(EMITE())->type = YOP_POP;
        }
        var = (YVarEntry *)YArray_pop(&c->vars);
    }

    YFuncEntry *func = (YFuncEntry *)YArray_pop(&c->funcs);
    while(!func->isMarker){
        func = (YFuncEntry *)YArray_pop(&c->funcs);
    }

    yfuncCall *call = (yfuncCall *)YArray_pop(&c->funcCalls);
    while(!call->isMarker){
        YFuncEntry *func = yyCompiler_findFunc(c,call->fname,0);
        if(func == NULL){
            yyCompiler_err(c,call->line,"invokation of undefined function %s",STR(call->fname));
            return -1;
        }
        else {
            yop *op = OP(call->opline);
            if(call->argCount != func->argCount){
                yyCompiler_err(c,call->line,"argument miss match for function %s: require %d,%d given",STR(call->fname),func->argCount,call->argCount);
                return -1;
            }
            op->arg1.i = func->entry;
        }
        call = (yfuncCall *)YArray_pop(&c->funcCalls);
    }
    return 0;
}
int yyCompiler_doExit(yyCompiler *c){
    OP(EMITE())->type = YOP_EXIT;
    return 0;
}
static YVarEntry *yyCompiler_addVar(yyCompiler *c){
    YVarEntry *v = (YVarEntry *)YArray_push(&c->vars);
    YVarEntry *last = (YVarEntry *)YArray_get(&c->vars,c->vars.len - 2);
    if(last->entryType == YVARENTRY_MARKER){
        v->index = 0;
    }
    else {
        v->index = last->index + 1;
    }
    return v;
}
int yyCompiler_doDeclVar(yyCompiler *c,const yynode *var,yyvartype_t type,y_entry_type_t entryType){
    YVarEntry *vare = yyCompiler_findVar(c,var->u.image.s,1);
    if(vare != NULL){
        yyCompiler_err(c,var->line,"redeclare of variable %s",STR(var->u.image.s));
        return -1;
    }
    else{
        vare = yyCompiler_addVar(c);
        vare->name = var->u.image.s;
        vare->type = type;
        vare->level = c->scopeLevel;  
        vare->entryType = entryType;
        if(entryType == YVARENTRY_VAR){
            OP(EMITE())->type = YOP_PUSH;
        }   
        return 0;
    }
}
int yyCompiler_doGetVar(yyCompiler *c,const yynode *var,yynode *ret){
    YVarEntry *sym = yyCompiler_findVar(c,var->u.image.s,0);
    if(sym == NULL){
        yyCompiler_err(c,var->line,"use of undefined variable symbol %s",STR(var->u.image.s));
        return -1;
    }
    else {
        ret->type = YY_IS_VAR;
        ret->u.varPos = sym->index;
        return 0;
    }
}
int yyCompiler_doConstant(yyCompiler *c,const yynode *n,yyvartype_t type,yynode *ret){
    if(type == YY_TYPE_INT){
        ret->varType = type;
        ret->type = YY_IS_CONST;
        if(type == YY_TYPE_INT){
            ret->u.i = n->u.i;
        }
        else{
            assert(0);
        }
    }
    else {
        assert(0);
    }
    return 0;
}
int yyCompiler_emitVal(yyCompiler *c,const yynode *var){
    if(var->type == YY_IS_CONST){
        if(var->varType == YY_TYPE_INT){
            yop *op = OP(EMITE());
            op->type = YOP_ICONST;
            op->arg1.i = var->u.i;
        }
        else {
            assert(0);
        }
    }
    else if(var->type == YY_IS_VAR){
        yop *op = OP(EMITE());
        op->type = YOP_LOAD;
        op->arg1.i = var->u.varPos;
    }
    return 0;
}
int yyCompiler_doBinaryOp(yyCompiler *c,yop_t op,const yynode *op1,const yynode *op2,yynode *ret){
    if(op1->type == YY_IS_CONST && op2->type == YY_IS_CONST){
        ret->varType = op1->varType;
        ret->type = YY_IS_CONST;
        switch(op){
            case YOP_PLUS: ret->u.i = op1->u.i + op2->u.i; break;
            case YOP_MINUS: ret->u.i = op1->u.i - op2->u.i; break;
            case YOP_TIMES: ret->u.i = op1->u.i * op2->u.i; break;
            case YOP_DIVIDE: ret->u.i = op1->u.i / op2->u.i; break;
            case YOP_MOD: ret->u.i = op1->u.i % op2->u.i; break;
            case YOP_LT: ret->u.i = op1->u.i < op2->u.i; break;
            case YOP_GT: ret->u.i = op1->u.i > op2->u.i; break;
            case YOP_LTOE: ret->u.i = op1->u.i <= op2->u.i; break;
            case YOP_GTOE: ret->u.i = op1->u.i >= op2->u.i; break;
            case YOP_EQ: ret->u.i = op1->u.i == op2->u.i; break;
            case YOP_NEQ: ret->u.i = op1->u.i != op2->u.i; break;
            default:
                assert(0);
        }
        return 0;
    }
    else {
        yyCompiler_emitVal(c,op1);
        yyCompiler_emitVal(c,op2);
        OP(EMITE())->type = op;
        ret->type = YY_IS_TEMP;
        ret->varType = op1->varType;

    }
}
int yyCompiler_doAssign(yyCompiler *c,const yynode *var,const yynode *val){
    yyCompiler_emitVal(c,val);
    assert(var->type == YY_IS_VAR);
    yop *op = OP(EMITE());
    op->type = YOP_STORE;
    op->arg1.i = var->u.varPos;
    return 0;
}

int yyCompiler_doBeginFuncDecl(yyCompiler *c,yynode *funcToken,const yynode *fname){
    YFuncEntry *func = yyCompiler_findFunc(c,fname->u.image.s,1);
    if(func != NULL){
        yyCompiler_err(c,fname->line,"function %s is already declared in this scope",STR(fname->u.image.s));
        return -1;
    }
    func = (YFuncEntry *)YArray_push(&c->funcs);
    func->isMarker = 0;
    func->name = fname->u.image.s;
    func->type = YY_TYPE_INT;
    func->argCount = 0;
    OP(EMITE())->type = YOP_JMP;
    func->entry = EMITE();
    OP(func->entry)->type = YOP_SUBVSP;

    yyCompiler_doEnterScope(c);
    funcToken->u.ptr = func;

    // the function return value
    yyCompiler_doDeclVar(c,fname,func->type,YVARENTRY_FUNC);
    
    return 0;
}
int yyCompiler_doAddFuncArg(yyCompiler *c,const yynode *var,yyvartype_t type){
    if(yyCompiler_doDeclVar(c,var,type,YVARENTRY_ARG)){
        return -1;
    }

    return 0;
}
int yyCompile_doEndFuncArgDef(yyCompiler *c,yynode *funcToken,const yynode *funcName,const yynode *args){
    YFuncEntry *func = (YFuncEntry *)funcToken->u.ptr;
    func->argCount = args->u.i;
    OP(func->entry)->arg1.i = func->argCount;
    return 0;
}
int yyCompiler_doEndFuncDecl(yyCompiler *c,yynode *funcToken){
    yyCompiler_doLeaveScope(c);
    OP(EMITE())->type = YOP_RET;
    YFuncEntry *func = (YFuncEntry *)funcToken->u.ptr;
    OP(func->entry - 1)->arg1.i = c->opLen;
    return 0;
}
int yyCompiler_doPrepareFuncCall(yyCompiler *c){
    // allocate a slot for the function return value.
    OP(EMITE())->type = YOP_PUSH;
    return 0;
}
int yyCompiler_doFuncCall(yyCompiler *c,const yynode *func,const yynode *args,yynode *ret){
    YFuncEntry *f = (YFuncEntry *)yyCompiler_findFunc(c,func->u.image.s,0);
    ret->type = YY_IS_TEMP;
    
    if(f == NULL){
        yfuncCall *fc = YArray_push(&c->funcCalls);
        ret->varType = YY_TYPE_ANY;
        fc->isMarker = 0;
        fc->line = func->line;
        fc->fname = func->u.image.s;
        fc->argCount = args->u.i;
        fc->opline = EMITE();
        OP(fc->opline)->type = YOP_INVOKE;
    }
    else {
        if(f->argCount != args->u.i){
            yyCompiler_err(c,args->line,"arguments miss match: require %d,%d given",f->argCount,args->u.i);
            return -1;
        }
        ret->varType = YY_TYPE_ANY;
        yop *op = OP(EMITE());
        op->type = YOP_INVOKE;
        op->arg1.i = f->entry;
        return 0;
    }
}
int yyCompiler_doRead(yyCompiler *c,const yynode *var){
    OP(EMITE())->type = YOP_READ;
    yop *op = OP(EMITE());
    op->type = YOP_STORE;
    op->arg1.i = var->u.varPos;
    return 0;
}
int yyCompiler_doWrite(yyCompiler *c,const yynode *val){
    yyCompiler_emitVal(c,val);
    OP(EMITE())->type = YOP_WRITE;
    return 0;
}

int yyCompiler_doIfCond(yyCompiler *c,yynode *ifToken,const yynode *cond){
    yyCompiler_emitVal(c,cond);
    ifToken->u.opline = c->opLen;
    yop *op = OP(EMITE());
    op->type = YOP_JMP_Z;
    return 0;
}
int yyCompiler_doEndIf(yyCompiler *c,yynode *ifToken){
    OP(ifToken->u.opline)->arg1.i = c->opLen;
    return 0;
}
int yyCompiler_doElse(yyCompiler *c,yynode *ifToken){
    yopline_t elseJmp = c->opLen;
    OP(EMITE())->type = YOP_JMP;
    OP(ifToken->u.opline)->arg1.i = c->opLen;
    ifToken->u.opline = elseJmp;
    return 0;
}
int yyCompiler_doEndElse(yyCompiler *c,yynode *ifToken){
    OP(ifToken->u.opline)->arg1.i = c->opLen;
    return 0;
}

int yyCompiler_halt(yyCompiler *c){
    yyCompiler_doLeaveScope(c);
    yyCompiler_doExit(c);
    c->halted = 1;
    return 0;
}
