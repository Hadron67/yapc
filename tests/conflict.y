
%token <+> "PLUS"
%token <*> "MULTIPLY"
%token <id> "ID"

%%

start: expr;

expr: expr <+> expr | expr <*> expr | atom;

atom: <id>;

%%

