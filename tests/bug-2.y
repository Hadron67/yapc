/*
    note: this bug has been fixed in v0.1.2

    this bug is due to that the function YItemSet_mergeTo doesn't work properly,
    it only merge the first item in two item sets!

*/
%token <ID> "ID"//
%token <NUM> "NUM"//
%token <-> "MINUS"//
%token <+> "PLUS"//
%token <[> "CBRA"
%token <]> "CKET"

%%

simpleExpr:
    sign factor;

sign: /* empty */;

factor:
    <NUM> | variable;

variable:
    <ID> <[> simpleExpr <]>;

%test 
    <ID> <[> <NUM> <]>;

%%
