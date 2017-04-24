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
#include <stdio.h>
#include "grammar.h"
#include "spool.h"

typedef struct _YRawRuleItem{
    int isTerminal;
    //int hasAction;
    size_t name;
    //size_t actionBlock;
}YRawRuleItem;

typedef struct _YRawRule{
    struct _YRawRule *next;
    size_t lhs;

    size_t actionBlock;
    int line;
    int hasAction;

    int hasValue;
    int stackOffset;
    int isGen;
    
    size_t iLen,iSize;
    YRawRuleItem items[1];
}YRawRule;

typedef struct _YGBuilder{
    YSPool pool;

    YSTree tokenEntry,ntEntry,aliasEntry;

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

    size_t nameSpace;
    size_t tokenPrefix;
    size_t stype;
    size_t dataType;

    size_t prologue;

    int addedNtCount;
}YGBuilder;

int YGBuilder_init(YGBuilder *gb);
int YGBuilder_reInit(YGBuilder *gb);
int YGBuilder_free(YGBuilder *gb,char **spool);
size_t YGBuilder_addString(YGBuilder *gb,const char *s);
int YGBuilder_setPrologue(YGBuilder *gb,const char *prologue);
int YGBuilder_setTokenType(YGBuilder *gb,const char *type);
int YGBuilder_setTokenPrefix(YGBuilder *gb,const char *tp);
int YGBuilder_setNameSpace(YGBuilder *gb,const char *ns);
int YGBuilder_setDataType(YGBuilder *gb,const char *type);
int YGBuilder_addToken(YGBuilder *gb,const char *tk,const char *alias);
int YGBuilder_prepareRule(YGBuilder *gb,const char *lhs);
int YGBuilder_commitRule(YGBuilder *gb);
int YGBuilder_addRuleItem(YGBuilder *gb,const char *name,int isTerminal);
int YGBuilder_addBlockItem(YGBuilder *gb,const char *action,int line);
int YGBuilder_addTestToken(YGBuilder *gb,const char *tname);
int YGBuilder_commitTest(YGBuilder *gb);
YGrammar *YGBuilder_build(YGBuilder *gb,FILE *err);

#endif
