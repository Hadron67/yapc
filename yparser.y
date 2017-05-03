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

    if you modify this file,please regenerate the parser by running

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

%token <%left> "LEFT_DIR"
%token <%right> "RIGHT_DIR"
%token <%nonassoc> "NONASSOC_DIR"
%token <%prec> "PREC_DIR"

%token <%namespace> "NS_DIR"

%token <%%> "SEPERATOR"
%token <:> "ARROW"
%token <|> "OR"
%token <;> "EOL"
%token <-> "NEG"

%token <name> "NAME"
%token <NUM> "NUM"
%token <token> "TOKEN"
%token <string> "STRING"
%token <block> "BLOCK"
%token <prologue> "PROLOGUE"

%type "YToken"

%datatype "YGBuilder"

%%

file: prologue options <%%> body <%%>;

options: options option | /* empty */ ;

option: 
    <%token> tokenDefs 
|   <%type> <string> { yydata->stype = $2.image; }
|   <%datatype> <string> { yydata->dataType = $2.image; }
|   <%token_prefix> <string> { yydata->tokenPrefix = $2.image; }
|   <%namespace> <string> { yydata->nameSpace = $2.image; }
|   <%enable_cst> { yydata->genCst = 1; }
|   <%left> incPr leftTokens
|   <%right> incPr rightTokens
|   <%nonassoc> incPr nonassocTokens
;

prologue: <prologue> { yydata->prologue = $1.image; } | /* empty */ ;

tokenDefs:
    tokenDefs <token> <string> { YGBuilder_addToken(yydata,&$2,&$3); }
|   <token> <string> { YGBuilder_addToken(yydata,&$1,&$2); }
;

incPr: { yydata->prLevel++; };

leftTokens: 
    leftTokens <token> { YGBuilder_setTokenPrecedence(yydata,&$2,YP_LEFT); }
|   <token> { YGBuilder_setTokenPrecedence(yydata,&$1,YP_LEFT); }
;
rightTokens: 
    rightTokens <token> { YGBuilder_setTokenPrecedence(yydata,&$2,YP_RIGHT); }
|   <token> { YGBuilder_setTokenPrecedence(yydata,&$1,YP_RIGHT); }
;
nonassocTokens: 
    nonassocTokens <token> { YGBuilder_setTokenPrecedence(yydata,&$2,YP_NONASSOC); }
|   <token> { YGBuilder_setTokenPrecedence(yydata,&$1,YP_NONASSOC); }
;

body: body bodyItem | bodyItem;

bodyItem: compoundRule <;> | test <;>;

test: <%test> tokenList { YGBuilder_commitTest(yydata); } ;

tokenList: tokenList <token> { YGBuilder_addTestToken(yydata,&$2); } | /* empty */ ;

compoundRule: 
    <name> { yydata->lhs = $1.image;YGBuilder_prepareRule(yydata,yydata->lhs); } 
    <:> ruleBody
;

ruleBody: 
    rule { YGBuilder_commitRule(yydata); }
|   ruleBody { YGBuilder_prepareRule(yydata,yydata->lhs); } 
    <|> rule { YGBuilder_commitRule(yydata); }
;

rule: ruleItems rulePrec ;

ruleItems: ruleItems ruleItem | <%empty> | /* empty */;

rulePrec:
    /* empty */
|   nonEmptyRulePrec
|   nonEmptyRulePrec <block> { YGBuilder_addBlockItem(yydata,$2.image,$2.line); }
;

nonEmptyRulePrec:
    <%prec> <token> { YGBuilder_setRulePrecedence(yydata,&$2,NULL); }
|   <%prec> <token> num { YGBuilder_setRulePrecedence(yydata,&$2,&$3); }
;

num: <NUM> | <-> <NUM> { $$.num = -$2.num; };

ruleItem: 
    <token> { YGBuilder_addRuleItem(yydata,$1.image,1); }
|   <name>  { YGBuilder_addRuleItem(yydata,$1.image,0); }
|   <block> { YGBuilder_addBlockItem(yydata,$1.image,$1.line); }
;

%%
