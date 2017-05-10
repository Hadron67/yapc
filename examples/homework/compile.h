#ifndef __COMPILE_H__
#define __COMPILE_H__

#include "token.h"
#include "spool.h"
#include "dynamic_array.h"
#include "execute.h"

typedef enum _yentrytype_t{
    YVARENTRY_MARKER = 1,
    YVARENTRY_VAR,
    YVARENTRY_ARG,
    YVARENTRY_FUNC
}y_entry_type_t;

typedef struct _YVarEntry{
    y_entry_type_t entryType;
    ysptr name;
    int level;
    int index;
    int type;
}YVarEntry;

typedef struct _YFuncEntry{
    int isMarker;
    ysptr name;
    yyvartype_t type;
    int argCount;
    yopline_t entry;
}YFuncEntry;


typedef struct _yyCompiler{
    int scopeLevel;

    YArray vars,funcs;
    YArray funcCalls;

    yop *oparray;
    yopline_t opLen,opSize;

    YSPool *pool;   

    FILE *err;
    int status;

    int halted;
}yyCompiler;

int yyCompiler_init(yyCompiler *c,FILE *err,YSPool *pool);
int yyCompiler_free(yyCompiler *c);

int yyCompiler_doEnterScope(yyCompiler *c);
int yyCompiler_doLeaveScope(yyCompiler *c);
int yyCompiler_doExit(yyCompiler *c);

// functions for compiling,used in action blocks in parser.y
int yyCompiler_doDeclVar(yyCompiler *c,const yynode *var,yyvartype_t type,y_entry_type_t entryType);
int yyCompiler_doGetVar(yyCompiler *c,const yynode *var,yynode *ret);
int yyCompiler_doConstant(yyCompiler *c,const yynode *n,yyvartype_t type,yynode *ret);
int yyCompiler_emitVal(yyCompiler *c,const yynode *var);
int yyCompiler_doBinaryOp(yyCompiler *c,yop_t op,const yynode *op1,const yynode *op2,yynode *ret);
int yyCompiler_doAssign(yyCompiler *c,const yynode *var,const yynode *val);

int yyCompiler_doBeginFuncDecl(yyCompiler *c,yynode *funcToken,const yynode *fname);
int yyCompiler_doAddFuncArg(yyCompiler *c,const yynode *var,yyvartype_t type);
int yyCompile_doEndFuncArgDef(yyCompiler *c,yynode *funcToken,const yynode *funcName,const yynode *args);
int yyCompiler_doEndFuncDecl(yyCompiler *c,yynode *funcToken);

int yyCompiler_doPrepareFuncCall(yyCompiler *c);
int yyCompiler_doFuncCall(yyCompiler *c,const yynode *func,const yynode *args,yynode *ret);

int yyCompiler_doRead(yyCompiler *c,const yynode *var);
int yyCompiler_doWrite(yyCompiler *c,const yynode *var);

int yyCompiler_doIfCond(yyCompiler *c,yynode *ifToken,const yynode *cond);
int yyCompiler_doEndIf(yyCompiler *c,yynode *ifToken);
int yyCompiler_doElse(yyCompiler *c,yynode *ifToken);
int yyCompiler_doEndElse(yyCompiler *c,yynode *ifToken);

int yyCompiler_halt(yyCompiler *c);

#endif
