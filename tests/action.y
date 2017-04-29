/*
    just like the strategy used in bison,yapc would create an extra non-terminal and a rule when an action appear
    in the middle of a rule,and assign that action to the newly added rule,since we want to treat all 
    actions as reduction actions.
    
    for example,the rule:

    expr: expr <+> { printf("i saw a plus"); } { printf("haha"); } atom

    would become:

    expr: expr <+> @1 $@2 atom
    @1: { printf("i saw a plus"); }
    $@2: { printf("haha"); }

    where "@1" and "$@2" are added non terminals.
    
*/
%token <id> "ID"
%token <+> "PLUS"
%token <num> "NUM"
%token <num> "s"

%%

start: expr { printf("accept");$$ = $1; };

expr: expr <+> { printf("i saw a plus");$$ = 0; } atom { $$ = $1 + $3; } | atom { $$ = $1; };

atom: { printf("i'm expecting an id"); } { printf("a new node"); } <id> { $$ = $1; } | { printf("i'm expecting a num"); } <num> { $$ = $1; };

%test <num> <+> <id> <+> <num> <+> <id>;

%%
