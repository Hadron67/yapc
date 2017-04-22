
%token <{> "OPEN_CURLY"
%token <}> "CLOSE_CULRY"
%token <:> "COLLON"
%token <,> "COMMA"
%token <[> "CBRA"
%token <]> "CKET"

%token <num> "NUM"
%token <id> "ID"
%token <true> "TRUE"
%token <false> "FALSE"
%token <string> "STRING"

%%

json: <{> obj <}> | <[> array <]> | atom;

obj: nonEmptyObj possibleComma | /* empty */;
nonEmptyObj: nonEmptyObj <,> objItem | objItem;
objItem: objKey <:> json;
objKey: <id> | <string>;

array: nonEmptyArray possibleComma | /* empty */;
nonEmptyArray: nonEmptyArray <,> json | json;

atom: <num> | <true> | <false> | <string> | <id>;

possibleComma: <,> | /* empty */;

%test <{> <id> <:> <[> <num> <,> <num> <]> <,> <id> <:> <true> <}>;
%test <[> <id> <,> <{> <}> <,> <true> <,> <[> <num> <,> <id> <]> <,> <]> <]>;

%%
