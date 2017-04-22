
%token <+> "PLUS"
%token <-> "MINUS"
%token </> "DIVIDE"
%token <*> "TIMES"
%token <(> "BRA"
%token <)> "KET"
%token <id> "ID"
%token <num> "NUM"
%token <,> "COMMA"
%token <?> "ASK"
%token <:> "COLLON"
%token <;> "SEMI_COLLON"

%token <=> "EQU"
%token <+=> "PLUS_A"
%token <-=> "MINUS_A"
%token <*=> "TIMES_A"
%token </=> "DIVIDE_A"

%token <!> "NOT"
%token <[> "CBRA"
%token <]> "CKET"
%token <{> "CURLY_BRA"
%token <}> "CURLY_KET"
%token <arrow> "ARROW"

%token <if> "IF"
%token <else> "ELSE"
%token <while> "WHILE"
%token <for> "FOR"
%token <function> "FUNCTION"
%token <member> "MEMBER"

%%

start: statementList;

statementList: statementList statement | ;

statement: ifStatement <;> | forStatement <;>| whileStatement <;>| <{> statementList <}> | exprList <;> | <;>;

ifStatement: <if> { printf("before expression"); } <(> exprList <)> statement elseStatement;

elseStatement: <else> statement | ;


whileStatement: <while> <(> exprList <)> statement;
forStatement: <for> <(> forExpr <;> exprList <;> exprList <;> <)> statement;
forExpr: assignExpr;


exprList: exprList <,> assignExpr | assignExpr;

assignExpr: assignExpr assignOptr conditionalExpr | conditionalExpr ;

assignOptr: <=> | <+=> | <-=> | <*=> | </=>;

conditionalExpr: binaryExpr | binaryExpr <?> assignExpr <:> assignExpr;

binaryExpr: binaryExpr <+> multiplyExpr | binaryExpr <-> multiplyExpr | multiplyExpr;

multiplyExpr: multiplyExpr <*> unaryExpr { $$ = $1 + $3; } | multiplyExpr </> unaryExpr | unaryExpr;

unaryExpr: atomicExpr;

atomicExpr: atom trailer;

trailer: trailer <[> subscript <]> | trailer <(> argList <)> | trailer <member> | ;

atom: <id> | <num> | <(> exprList <)> | lambda | aFunction;

lambda: lambdaHead <arrow> lambdaBody;
lambdaHead: <id> | <(> argList <)>;
lambdaBody: assignExpr | <{> statementList <}>;

argList: nonEmptyArgList | ;
nonEmptyArgList: nonEmptyArgList <,> assignExpr | assignExpr;

argDefList: nonEmptyArgDefList | ;
nonEmptyArgDefList: nonEmptyArgDefList <,> argDef | argDef;
argDef: <id>;

subscript: exprList | ;

aFunction: <function> funcName <(> argDefList <)> <{> statementList <}>;
funcName: <id> | ;

%%
