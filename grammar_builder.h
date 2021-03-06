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

#ifndef __GRAMMAR_BUILDER_H__
#define __GRAMMAR_BUILDER_H__

#include "ydef.h"
#include <stdio.h>
#include "grammar.h"
#include "spool.h"

typedef struct _YToken{
    int line;
    int id;
    int num;
    ysptr image;
}YToken;

typedef struct _YRawRuleItem{
    int isTerminal;
    //int hasAction;
    ysptr name;
    //size_t actionBlock;
}YRawRuleItem;

typedef struct _YRawRule{
    struct _YRawRule *next;
    ysptr lhs;

    ysptr actionBlock;
    int line;
    int hasAction;

    int hasValue;
    int stackOffset;
    int isGen;

    int prLevel;
    
    size_t iLen,iSize;
    YRawRuleItem items[1];
}YRawRule;

typedef struct _YRawToken{
    ysptr name,alias;
    yprecedence_t pr;
    int prLevel;
}YRawToken;

typedef struct _YGBuilder{
    yheap_t *heap;
    YSPool pool;
    FILE *err;
    int status;

    YTree tokenEntry,ntEntry;

    size_t ruleSize;
    YRawRule *startRule,*ruleHead;
    YRawRule *currentRule;
    size_t itemCount,ruleCount;

    int actionCount;
    
    int *tests;
    size_t currentTest;
    int testLen,testSize,testCount;

    int first;
    int built;

    ysptr nameSpace;
    ysptr tokenPrefix;
    ysptr stype;
    ysptr dataType;

    ysptr prologue;

    int genCst;

    int addedNtCount;

    //following is used by the parser
    ysptr lhs;
    int prLevel;
}YGBuilder;

int YGBuilder_init(YGBuilder *gb,FILE *err,yheap_t *heap);
int YGBuilder_reInit(YGBuilder *gb);
int YGBuilder_free(YGBuilder *gb,char **spool);
ysptr YGBuilder_addString(YGBuilder *gb,const char *s);
const char *YGBuilder_getString(YGBuilder *gb,ysptr s);

int YGBuilder_addToken(YGBuilder *gb,const YToken *tk,const YToken *alias);
int YGBuilder_setTokenPrecedence(YGBuilder *gb,const YToken *tk,yprecedence_t p);
int YGBuilder_setRulePrecedence(YGBuilder *gb,const YToken *prec,const YToken *rel);
int YGBuilder_prepareRule(YGBuilder *gb,ysptr lhs);
int YGBuilder_commitRule(YGBuilder *gb);
int YGBuilder_addRuleItem(YGBuilder *gb,ysptr name,int isTerminal);
int YGBuilder_addBlockItem(YGBuilder *gb,ysptr block,int line);
int YGBuilder_addTestToken(YGBuilder *gb,const YToken *tk);
int YGBuilder_commitTest(YGBuilder *gb);
YGrammar *YGBuilder_build(YGBuilder *gb);

#endif
