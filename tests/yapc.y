%{

#include <stdio.h>
#include "grammar_builder.h"
#include "grammar.h"
#include "spool.h"

%}

%token <%token> "TOKEN_DIR"
%token <%type> "TYPE_DIR"
%token <%datatype> "DATATYPE_DIR"
%token <%test> "TEST_DIR"
%token <%empty> "EMPTY_DIR"
%token <%%> "SEPERATOR"
%token <:> "ARROW"
%token <|> "OR"
%token <;> "EOL"

%token <name> "NAME"
%token <token> "TOKEN"
%token <string> "STRING"
%token <block> "BLOCK"
%token <prologue> "PROLOGUE"

%type "ysptr"

%datatype "YGBuilder"

%%

file: prologue options <%%> body <%%>;

options: options option | /* empty */;

option: 
    <%token> <token> <string> { YGBuilder_addToken(yydata,$2,$3); } |
    <%type> <string> { YGBuilder_setTokenType(yydata,$2); } |
    <%datatype> <string> { YGBuilder_setDataType(yydata,$2); } ;

prologue: <prologue> | /* empty */;

body: body bodyItem | bodyItem;

bodyItem: rule <;> | test <;>;

test: <%test> tokenList;

tokenList: tokenList <token> | /* empty */;

rule: <name> <:> ruleBody;

ruleBody: ruleItems | ruleBody <|> ruleItems;

ruleItems: ruleItems ruleItem | /* empty */;

ruleItem: <token> | <name> | <block>;

%%
