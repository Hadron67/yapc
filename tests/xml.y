%token <gt> "GT"
%token <lt> "LT"
%token <=> "EQU"
%token <:> "COLLON"
%token <lt?> "LT_ASK"
%token <gt?> "GL_ASK"
%token </> "SLASH"

%token <id> "ID"
%token <string> "STRING"
%token <content> "CONTENT"

%%

xml: head body;

head: <lt?> <id> properties <gt?> |/* empty */;

body: tag;

tag:
    <lt> <id> properties <gt> innerTag <lt> </> <id> <gt> |
    <lt> <id> properties </> <gt>;

innerTag: tag | <content> | /* empty */;

properties: properties property | /* empty */;

property: propertyName | propertyName <=> <string>;

propertyName: <id> <:> <id> | <id>;

%test <lt> <id> <id> <=> <string> <gt> <lt> <id> <id> <=> <string> <gt> <lt> <id> </> <gt> <lt> <id> </> <gt>;

%%
