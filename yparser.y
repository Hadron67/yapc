/*  
    YAPC - Yet Another Parser Compiler - An LR(1) parser generator

    Copyright (C) 2017  Chen FeiYu

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* 
    this grammar file uses yapc to generate the grammar file parser for yapc itself,
    it may seems strange to do so,but,...,it's ok.

    if you change this file,please regenerate the parser by running

    ./yapc yparser.y

*/


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
%token <%token_prefix> "TOKEN_PREFIX_DIR"
%token <%enable_cst> "ENABLE_CST_DIR"
%token <%ns> "NS_DIR"
%token <%%> "SEPERATOR"
%token <:> "ARROW"
%token <|> "OR"
%token <;> "EOL"

%token <name> "NAME"
%token <token> "TOKEN"
%token <string> "STRING"
%token <block> "BLOCK"
%token <prologue> "PROLOGUE"

%type "YToken"

%datatype "YGBuilder"

%%

file: prologue options <%%> body <%%>;

options: options option | /* empty */;

option: 
    <%token> <token> <string> { YGBuilder_addToken(yydata,$2.image,$3.image); } |
    <%type> <string> { yydata->stype = $2.image; } |
    <%datatype> <string> { yydata->dataType = $2.image; } |
    <%token_prefix> <string> { yydata->tokenPrefix = $2.image; } |
    <%ns> <string> { yydata->nameSpace = $1.image; } |
    <%enable_cst> { yydata->genCst = 1; } ;

prologue: <prologue> { yydata->prologue = $1.image; } | /* empty */;

body: body bodyItem | bodyItem;

bodyItem: rule <;> | test <;>;

test: <%test> tokenList;

tokenList: tokenList <token> { YGBuilder_addTestToken(yydata,$2.image); } | /* empty */;

rule: 
    <name> { yydata->lhs = $1.image;YGBuilder_prepareRule(yydata,yydata->lhs); } <:> ruleBody;

ruleBody: 
    ruleItems { YGBuilder_commitRule(yydata); }| 
    ruleBody { YGBuilder_prepareRule(yydata,yydata->lhs); } <|> ruleItems { YGBuilder_commitRule(yydata); };

ruleItems: ruleItems ruleItem | /* empty */;

ruleItem: 
    <token> { YGBuilder_addRuleItem(yydata,$1.image,1); }| 
    <name>  { YGBuilder_addRuleItem(yydata,$1.image,0); }| 
    <block> { YGBuilder_addBlockItem(yydata,$1.image,$1.line); };

%%
