%{
#include <stdio.h>
#include <stdlib.h>

%}

%token <num> "NUM"

%token <+> "PLUS"
%token <-> "MINUS"
%token <*> "MULTIPLY"
%token </> "DIVIDE"
%token <(> "BRA"
%token <)> "KET"

%type "double"

%%

start:expr { printf("result: %lf\n",$1); } ;

expr: 
    expr <+> { printf("i saw a plus\n"); } multiplyExpr { $$ = $1 + $3; } | 
    expr <-> multiplyExpr { $$ = $1 - $3; } |
    multiplyExpr { $$ = $1; } ;
    
multiplyExpr:
    multiplyExpr <*> atomicExpr { $$ = $1 * $3; } |
    multiplyExpr </> atomicExpr { $$ = $1 / $3; } |
    atomicExpr { $$ = $1; };

atomicExpr:
    atom { $$ = $1; } ;

atom:
    <num> { $$ = $1; } |
    <(> expr <)> { $$ = $2; } ;
    
%test <num><+><num> ;
%test <num><+><(><num><*><(><num><*><num><-><num><)><+><num><)>;

%%
