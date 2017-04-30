
%token <NUM> "NUM"
%token <+> "PLUS"
%token <-> "MINUS"
%token <*> "TIMES"


%left <+> <->
/*
    if '+' is declared as right associative,the second test won't be affected
    but the first one would have a difference parse tree.
*/
// %right <+>


/*
    the first test results in syntax error if '+' is declared as non-associative.
*/
// %nonassoc <+>

// '*' has a higher precedence level than '+'
%left <*>

%%

expr:
    expr <+> expr |
    expr <-> expr |
    expr <*> expr |
    <+> expr |
    <-> expr |
    <NUM>;

%test <NUM> <+> <NUM> <*> <-> <NUM> <+> <NUM>;

%test <NUM> <+> <NUM> <*> <NUM>;

%%