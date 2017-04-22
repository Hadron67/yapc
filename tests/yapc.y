
%token <%token> "TOKEN_DIR"
%token <%test> "TEST_DIR"
%token <%%> "SEPERATOR"
%token <:> "COLLON"
%token <|> "OR"
%token <;> "SEMI_COLLON"

%token <id> "ID"
%token <token> "TOKEN"
%token <string> "STRING"
%token <block> "BLOCK"

%%

file: options <%%> body <%%>;

options: options option | /* empty */;

option: <%token> <token> <string>;

body: body bodyItem | bodyItem;

bodyItem: rule | test;

test: <%test> tokenList <;>;

tokenList: tokenList <token> | /* empty */;

rule: <id> <:> ruleBody;

ruleBody: ruleItems | ruleBody <|> ruleItems;

ruleItems: ruleItems ruleItem | /* empty */;

ruleItem: <token> | <id> | <block>;

%%
