%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
%}

%token <num> "NUM"

%token <+> "PLUS"
%token <-> "MINUS"
%token <*> "MULTIPLY"
%token </> "DIVIDE"
%token <**> "EXP"
%token <(> "BRA"
%token <)> "KET"

%token <sin> "SIN"
%token <cos> "COS"
%token <tan> "TAN"
%token <asin> "ASIN"
%token <acos> "ACOS"
%token <atan> "ATAN"
%token <exp> "CEXP"
%token <ln> "LN"

%token <I> "I"
%token <pi> "PI"
%token <e> "E"

%type "double _Complex"

%datatype "int"

%%

start:expr { $$ = $1; } ;

expr: 
    expr <+> multiplyExpr { $$ = $1 + $3; } | 
    expr <-> multiplyExpr { $$ = $1 - $3; } |
    multiplyExpr { $$ = $1; } ;
    
multiplyExpr:
    multiplyExpr <*> powerExpr { $$ = $1 * $3; } |
    multiplyExpr </> powerExpr { $$ = $1 / $3; } |
    powerExpr { $$ = $1; };
    
powerExpr:
    powerExpr <**> atomicExpr { $$ = cpow($1,$3); } |
    atomicExpr { $$ = $1; };
    
atomicExpr:
    atom { $$ = $1; } |
    <+> atom { $$ = $2; } |
    <-> atom { $$ = -$2; } |
    //complex conjugate,this notation is used frequently in quantum mechanics.
    <*> atom { $$ = conj($2); };

atom:
    <num> { $$ = $1; } |
    <(> expr <)> { $$ = $2; } |
    functions { $$ = $1; } |
    consts { $$ = $1; } ;
    
functions:
    <sin> <(> expr <)> { $$ = csin($3); } |
    <cos> <(> expr <)> { $$ = ccos($3); } |
    <tan> <(> expr <)> { $$ = ctan($3); } |
    <asin> <(> expr <)> { $$ = casin($3); } |
    <acos> <(> expr <)> { $$ = cacos($3); } |
    <atan> <(> expr <)> { $$ = catan($3); } |
    <exp> <(> expr <)> { $$ = cexp($3); } |
    <ln> <(> expr <)> { $$ = clog($3); };
    
consts:
    <I> { $$ = I; } |
    <pi> { $$ = 3.14159265358979; } |
    <e> { $$ = 2.718281828; };
    
%test <num><+><num> ;
%test <num><+><(><num><*><(><num><*><num><-><num><)><+><num><)>;

%%
