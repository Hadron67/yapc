
%token  <+> "PLUS"
%token  <*> "TIMES"
%token  <(> "BRA"
%token <)> "KET"
%token <id> "ID"

%%

Expr    : Expr <+> Term;
Expr    : Term;
Term    : Term <*> Factor;
Term    : Factor;
Factor  : <(> Expr <)>;
Factor  : <id>;

%test <id> <+> <id> <*> <(> <id> <*> <id> <+> <id> <)>;

%%
