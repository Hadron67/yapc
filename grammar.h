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

#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <stdio.h>
#include "ydef.h"

typedef struct _YRuleItem YRuleItem;
typedef struct _YRule YRule;

struct _YRuleItem{
    int id;
    int isTerminal;
};

struct _YRule{
    int index;
    int lhs;
    int length;
    int line;
    const char *actionBlock;
    YRule *next; //next rule with the same LHS

    //hasValue only makes sense when isGen is true,
    //i.e.,user-defined rules always has a value.
    int hasValue,isGen;

    //the number of sematic variables on the stack when this rule is recongnized.
    
    int stackOffset;
    
    //precedence level,used to resolve conflicts.
    int prLevel;
    YRuleItem *rule;
};

typedef enum _yprecedence_t{
    YP_NONE = 1,
    YP_LEFT,
    YP_RIGHT,
    YP_NONASSOC
}yprecedence_t;

typedef struct _YTokenDef{
    const char *name;
    const char *alias;
    int used;
    int prLevel;
    yprecedence_t pr;
}YTokenDef;

typedef struct _YNtDef{
    const char *name;
    YRule *firstRule;
    int used;
}YNtDef;

typedef struct _YGrammar{
    // heap that allocated this grammar
    yheap_t *heap;
    // token definations
    YTokenDef *tokens;
    int tokenCount;

    // nonterminals
    YNtDef *nts;
    int ntCount;

    // rules
    int ruleCount;
    YRule *rules;

    // length of the longest rule
    int maxRuleLen;

    // test entries
    int *tests;
    int testCount;

    // total number of action block -- not used
    int actionCount;
    
    // MIS properties
    const char *nameSpace;
    const char *tokenPrefix;
    const char *stype;
    const char *dataType;
    const char *prologue;

    // whether to generate concrete syntax tree or not.
    int genCst;

    // symbol table
    char *spool;

    // actual data goes here,including tokens,nts,rules and symbol table.
    char data[1];
}YGrammar;

//keep in mind that the index of a terminal in this first set is its origin index plus 1,
//because token with index 0 is reserved for epsilon,i.e.,empty.
typedef struct _YFirstSets{
    YGrammar *g;
    int size;
    char data[1];
}YFirstSets;

int YRule_dump(YGrammar *g,YRule *rule,FILE *out);
int YGrammar_dump(YGrammar *g,FILE *out);
int YGrammar_free(YGrammar *g);

// token set (bit set) helper functions
int YGrammar_getTokenSetSize(YGrammar *g);
// here item set means LR(0) item sets,i.e.,without look-aheads.
int YGrammar_getItemSetSize(YGrammar *g);
int YTokenSet_add(YGrammar *g,char *set,int token);
int YTokenSet_removeToken(YGrammar *g,char *set,int token);
int YTokenSet_contains(YGrammar *g,char *set,int token);
int YTokenSet_union(YGrammar *g,char *dest,const char *src);
int YTokenSet_hasIntersection(YGrammar *g,char *s1,char *s2);
int YTokenSet_isIdentical(char *s1,char *s2,YGrammar *g);


YFirstSets *YGrammar_generateFirstSets(YGrammar *g);
int YGrammar_printUnusedTokens(YGrammar *g,int *count,FILE *out);
int YGrammar_printUnusedNts(YGrammar *g,int *count,FILE *out);

int YFirstSets_contains(YFirstSets *sets,int nt,int t);
char *YFirstSets_getSet(YFirstSets *sets,int nt);
int YFirstSets_dump(YFirstSets *sets,FILE *out);
int YFirstSets_free(YFirstSets *sets);

#endif
