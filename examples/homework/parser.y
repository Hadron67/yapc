%{
    #include "token.h"   
    #include "compile.h" 
%}

%token <begin> "BEGIN"//
%token <end> "END"//
%token <integer> "INTEGER"//
%token <if> "IF"//
%token <then> "THEN"//
%token <else> "ELSE"//
%token <function> "FUNCTION"//
%token <read> "READ"//
%token <write> "WRITE"//
%token <ID> "ID"//
%token <NUM> "NUM"//
%token <=> "EQ"//
%token <ne> "NE"//
%token <ltoe> "LTOE"//
%token <lt> "LT" //
%token <gtoe> "GTOE"//
%token <gt> "GT"//
%token <-> "MINUS"//
%token <*> "MULTIPLY"//
%token <:=> "ASSIGN"//
%token <(> "BRA"//
%token <)> "KET"//
%token <;> "SEMI_COLLON"//


%token <,> "COMMA"
%token <+> "PLUS"
%token </> "DIVIDE"
%token <mod> "MOD"

%token <__halt_compiler> "HALT_COMPILER"

%left <gt> <lt> <gtoe> <ltoe> <=> <ne>
%left <-> <+>
%left <*> </> <mod>

%type "yynode"

%datatype "yyCompiler"

%enable_cst

%%

//<程序>→<分程序>
program: subProgram { yyCompiler_doExit(yydata); };

//<分程序>→begin <说明语句表>；<执行语句表> end
subProgram: 
    <begin> 
    { yyCompiler_doEnterScope(yydata); } 
    declList topStmtList 
    { yyCompiler_doLeaveScope(yydata); } 
    <end>
;

topStmtList:
    topStmtList topStmt | topStmt;
topStmt:
    stmt
|   <__halt_compiler> <(> <)> <;> { yyCompiler_halt(yydata); }
;

//<说明语句表>→<说明语句>│<说明语句表> ；<说明语句>
declList: declList decl | /* empty */;

//<说明语句>→<变量说明>│<函数说明>
decl: varDecl <;> | funcDecl;

//<变量说明>→integer <变量>
varDecl: <integer> varID { yyCompiler_doDeclVar(yydata,&$2,YY_TYPE_INT,YVARENTRY_VAR); };

//<变量>→<标识符>
varID: <ID>;
//<标识符>→<字母>│<标识符><字母>│ <标识符><数字>
//<字母>→a│b│c│d│e│f│g│h│i│j│k│l│m│n│o │p│q │r│s│t│u│v│w│x│y│z
//<数字>→0│1│2│3│4│5│6│7│8│9

//<函数说明>→integer function <标识符>（<参数>）；<函数体>
funcDecl: 
    <integer> <function> <ID> { yyCompiler_doBeginFuncDecl(yydata,&$2,&$3); }
    <(> argDefList { yyCompile_doEndFuncArgDef(yydata,&$2,&$3,&$5); } <)> 
    <;> funcBody { yyCompiler_doEndFuncDecl(yydata,&$2); } 
;

//<参数>→<变量>
argDefList:
    nonEmptyArgDef | /* empty */ { $$.u.i = 0; } ;
nonEmptyArgDef:
    nonEmptyArgDef <,> varID  { $$.u.i = $1.u.i + 1;yyCompiler_doAddFuncArg(yydata,&$3,YY_TYPE_INT); }
|   varID { $$.u.i = 1;yyCompiler_doAddFuncArg(yydata,&$1,YY_TYPE_INT); }
;

//<函数体>→begin <说明语句表>；<执行语句表> end
funcBody: <begin> declList stmtList <end>;

//<执行语句表>→<执行语句>│<执行语句表>；<执行语句>
stmtList: stmt | stmtList stmt;

block:
    /* empty */
|   stmtList
;

//<执行语句>→<读语句>│<写语句>│<赋值语句>│<条件语句>
stmt:
    <begin> block <end>
|   readStmt <;> 
|   writeStmt <;> 
|   assignStmt <;> 
|   funcCall <;>
|   ifStmt
;

//<读语句>→read(<变量>)
readStmt: <read> <(> leftExpr <)> { yyCompiler_doRead(yydata,&$3); };

//<写语句>→write(<变量>)
writeStmt: <write> <(> expr <)> { yyCompiler_doWrite(yydata,&$3); };

//<赋值语句>→<变量>:=<算术表达式>
assignStmt: 
    leftExpr <:=> expr { yyCompiler_doAssign(yydata,&$1,&$3); }
;

leftExpr: varID { yyCompiler_doGetVar(yydata,&$1,&$$); } ;

//<算术表达式>→<算术表达式>-<项>│<项>
//<项>→<项>*<因子>│<因子>
//<因子>→<变量>│<常数>│<函数调用>
expr: 
    expr <+> expr { yyCompiler_doBinaryOp(yydata,YOP_PLUS,&$1,&$3,&$$); }
|   expr <-> expr { yyCompiler_doBinaryOp(yydata,YOP_MINUS,&$1,&$3,&$$); }
|   expr <*> expr { yyCompiler_doBinaryOp(yydata,YOP_TIMES,&$1,&$3,&$$); }
|   expr </> expr { yyCompiler_doBinaryOp(yydata,YOP_DIVIDE,&$1,&$3,&$$); }
|   expr <mod> expr { yyCompiler_doBinaryOp(yydata,YOP_MOD,&$1,&$3,&$$); }
|   expr <lt> expr { yyCompiler_doBinaryOp(yydata,YOP_LT,&$1,&$3,&$$); }
|   expr <gt> expr { yyCompiler_doBinaryOp(yydata,YOP_GT,&$1,&$3,&$$); }
|   expr <ltoe> expr { yyCompiler_doBinaryOp(yydata,YOP_LTOE,&$1,&$3,&$$); }
|   expr <gtoe> expr { yyCompiler_doBinaryOp(yydata,YOP_GTOE,&$1,&$3,&$$); }
|   expr <=> expr { yyCompiler_doBinaryOp(yydata,YOP_EQ,&$1,&$3,&$$); }
|   expr <ne> expr { yyCompiler_doBinaryOp(yydata,YOP_NEQ,&$1,&$3,&$$); }
|   constant 
|   varID { yyCompiler_doGetVar(yydata,&$1,&$$); }
|   funcCall
;

//<常数>→<无符号整数>
constant: 
    <NUM> { yyCompiler_doConstant(yydata,&$1,YY_TYPE_INT,&$$); } ;
//<无符号整数>→<数字>│<无符号整数><数字>
//<条件语句>→if<条件表达式>then<执行语句>else <执行语句>
ifStmt: 
    <if> expr { yyCompiler_doIfCond(yydata,&$1,&$2); } 
    <then> stmt <else> { yyCompiler_doElse(yydata,&$1); } 
    stmt { yyCompiler_doEndElse(yydata,&$1); }
;

funcCall: 
    <ID> { yyCompiler_doPrepareFuncCall(yydata); } 
    <(> argList <)> { yyCompiler_doFuncCall(yydata,&$1,&$3,&$$); } ;

argList:
    nonEmptyArgList | /* empty */ { $$.u.i = 0; };
nonEmptyArgList:
    nonEmptyArgList <,> expr { $$.u.i = $1.u.i + 1;yyCompiler_emitVal(yydata,&$3); }
|   expr { $$.u.i = 1;yyCompiler_emitVal(yydata,&$1); } ;

%%