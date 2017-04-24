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

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "grammar_builder.h"

int YGBuilder_init(YGBuilder *gb){
    memset(gb,0,sizeof(YGBuilder));
    YSPool_init(&gb->pool,32,32);
    YSTree_init(&gb->tokenEntry,32,&gb->pool);
    YSTree_init(&gb->ntEntry,32,&gb->pool);
    YSTree_init(&gb->aliasEntry,32,&gb->pool);
    gb->ruleSize = 8;
    gb->first = 1;

    gb->testSize = 16;
    gb->tests = (int *)ya_malloc(sizeof(int) * gb->testSize);
    gb->tests[gb->currentTest = 0] = 0;
    gb->testLen = 1;

    gb->addedNtCount = 1;

    gb->tokenPrefix = YSPool_addString(&gb->pool,"T_");
    gb->nameSpace = YSPool_addString(&gb->pool,"yy");
    gb->stype = YSPool_addString(&gb->pool,"int");
    gb->prologue = YSPool_addString(&gb->pool,"");
    gb->dataType = YSPool_addString(&gb->pool,"void");
    
    YGBuilder_addToken(gb,"EOF","EOF");

    return 0;
}
int YGBuilder_free(YGBuilder *gb,char **spool){
    YSTree_free(&gb->tokenEntry);
    YSTree_free(&gb->ntEntry);
    YSTree_free(&gb->aliasEntry);
    YSPool_free(&gb->pool,spool);

    if(!gb->built){
        ya_free(gb->tests);
    }

    YRawRule *raw = gb->startRule;
    while(raw != NULL){
        YRawRule *temp = raw->next;
        ya_free(raw);
        raw = temp;
    }
    if(gb->currentRule != NULL){
        ya_free(gb->currentRule);
    }

    return 0;
}
int YGBuilder_addToken(YGBuilder *gb,const char *tk,const char *alias){
    YSTree_prepareNode(&gb->tokenEntry);
    size_t *node = YSTree_find(&gb->tokenEntry,tk);
    if(*node == 0){
        size_t sp = YSPool_addString(&gb->pool,tk);
        *node = YSTree_newNode(&gb->tokenEntry,sp);
    }
    else {
        return -1;
    }

    YSTree_prepareNode(&gb->aliasEntry);
    node = YSTree_find(&gb->aliasEntry,alias);
    if(*node == 0){
        size_t sp = YSPool_addString(&gb->pool,alias);
        *node = YSTree_newNode(&gb->aliasEntry,sp);
    }
    else {
        return -1;
    }
    return 0;
}
int YGBuilder_setPrologue(YGBuilder *gb,const char *prologue){
    gb->prologue = YSPool_addString(&gb->pool,prologue);
    return 0;
}
int YGBuilder_setTokenType(YGBuilder *gb,const char *type){
    gb->stype = YSPool_addString(&gb->pool,type);
    return 0;
}
int YGBuilder_setTokenPrefix(YGBuilder *gb,const char *tp){
    gb->tokenPrefix = YSPool_addString(&gb->pool,tp);
    return 0;
}
int YGBuilder_setNameSpace(YGBuilder *gb,const char *ns){
    gb->nameSpace = YSPool_addString(&gb->pool,ns);
    return 0;
}
int YGBuilder_setDataType(YGBuilder *gb,const char *type){
    gb->dataType = YSPool_addString(&gb->pool,type);
    return 0;
}
size_t YGBuilder_addString(YGBuilder *gb,const char *s){
    return YSPool_addString(&gb->pool,type);
}
static YRawRule *YGBuilder_newRule(YGBuilder *gb){
    YRawRule *raw = (YRawRule *)ya_malloc(sizeof(YRawRule) + gb->ruleSize * sizeof(YRawRuleItem));
    raw->next = NULL;
    raw->iLen = 0;
    raw->iSize = gb->ruleSize;

    return raw;
}
static int YGBuilder_prepareRuleRaw(YGBuilder *gb,const char *lhs,int hasAction,size_t action,int line){
    if(gb->first){
        gb->first = 0;
        YGBuilder_prepareRule(gb,"(accept)");
        YGBuilder_addRuleItem(gb,lhs,0);
        YGBuilder_commitRule(gb);
    }
    YRawRule *rule = YGBuilder_newRule(gb);
    size_t *node = YSTree_find(&gb->ntEntry,lhs);
    size_t sp;
    if(*node == 0){
        sp = YSPool_addString(&gb->pool,lhs);
        *node = YSTree_newNode(&gb->ntEntry,sp);
    }
    else {
        sp = YSTree_getNode(&gb->ntEntry,*node)->sptr;
    }
    rule->lhs = sp;
    rule->actionBlock = action;
    rule->line = line;
    rule->hasAction = hasAction;
    rule->hasValue = 1;
    rule->stackOffset = 0;
    rule->isGen = 0;

    gb->currentRule = rule;

    return 0;
}
int YGBuilder_prepareRule(YGBuilder *gb,const char *lhs){
    YGBuilder_prepareRuleRaw(gb,lhs,0,0,0);
    return 0;
}
int YGBuilder_commitRule(YGBuilder *gb){
    assert(gb->currentRule != NULL);

    YRawRule *raw = gb->currentRule;
    if(gb->ruleHead != NULL){
        gb->ruleHead->next = raw;
    }
    gb->ruleHead = raw;
    if(gb->startRule == NULL){
        gb->startRule = raw;
    }
    gb->currentRule = NULL;

    gb->ruleCount++;

    return 0;
}

int YGBuilder_addRuleItem(YGBuilder *gb,const char *name,int isTerminal){
    YRawRule *raw = gb->currentRule;
    if(raw->iLen >= raw->iSize){
        raw->iSize *= 2;
        gb->currentRule = raw = (YRawRule *)ya_realloc(raw,sizeof(YRawRule) + raw->iSize * sizeof(YRawRuleItem));
    }
    if(raw->hasAction){
        raw->hasAction = 0;
        YRawRule *saved = gb->currentRule;
        char tname[10];
        snprintf(tname,10,"@%d",gb->addedNtCount++);

        YGBuilder_prepareRuleRaw(gb,tname,1,raw->actionBlock,raw->line);
        gb->currentRule->stackOffset = raw->stackOffset;
        gb->currentRule->hasValue = raw->hasValue;
        gb->currentRule->isGen = 1;
        if(!raw->hasValue){
            raw->stackOffset--;
        }

        YGBuilder_commitRule(gb);
        gb->currentRule = saved;
        YGBuilder_addRuleItem(gb,tname,0);
    }

    YRawRuleItem *item = raw->items + raw->iLen++;
    item->name = YSPool_addString(&gb->pool,name);
    item->isTerminal = isTerminal;

    gb->itemCount++;
    raw->stackOffset++;

    return 0;
}
int YGBuilder_addBlockItem(YGBuilder *gb,const char *action,int line){
    YRawRule *saved = gb->currentRule;
    if(saved->hasAction){
        char ntname[10];
        snprintf(ntname,10,"@%d",gb->addedNtCount++);
        YGBuilder_prepareRule(gb,ntname);
        YGBuilder_addBlockItem(gb,action,line);
        gb->currentRule->stackOffset = saved->stackOffset;
        gb->currentRule->isGen = 1;
        if(!gb->currentRule->hasValue){
            saved->stackOffset--;
        }
        YGBuilder_commitRule(gb);
        gb->currentRule = saved;
        YGBuilder_addRuleItem(gb,ntname,0);
    }
    else {
        saved->hasAction = 1;
        saved->actionBlock = YSPool_addString(&gb->pool,action);
        //FIXME: action still dont have a value if '$$' is in a string
        saved->hasValue = strstr(action,"$$") != NULL;
        saved->line = line;
    }
    return 0;
}
int YGBuilder_addTestToken(YGBuilder *gb,const char *tname){
    int t = *YSTree_find(&gb->tokenEntry,tname);
    if(t == 0){
        return -1;
    }
    if(gb->testLen >= gb->testSize){
        gb->testSize *= 2;
        gb->tests = (int *)ya_realloc(gb->tests,sizeof(int) * gb->testSize);
    }
    gb->tests[gb->testLen++] = t - 1;
    gb->tests[gb->currentTest]++;
    return 0;
}
int YGBuilder_commitTest(YGBuilder *gb){
    if(gb->testLen >= gb->testSize){
        gb->testSize *= 2;
        gb->tests = (int *)ya_realloc(gb->tests,sizeof(int) * gb->testSize);
    }
    gb->currentTest = gb->testLen++;
    gb->tests[gb->currentTest] = 0;
    gb->testCount++;
    return 0;
}

YGrammar *YGBuilder_build(YGBuilder *gb,FILE *err){
    assert(gb->tokenEntry.len == gb->aliasEntry.len);
    YGrammar *g = (YGrammar *)ya_malloc(
        sizeof(YGrammar) + 
        sizeof(const char *) * gb->tokenEntry.len +
        sizeof(const char *) * gb->tokenEntry.len +
        sizeof(const char *) * gb->ntEntry.len +
        sizeof(YRule) * gb->ruleCount + 
        sizeof(YRuleItem) * gb->itemCount
    );
    g->tokenCount = gb->tokenEntry.len - 1;
    g->ntCount = gb->ntEntry.len - 1;
    g->ruleCount = gb->ruleCount;
    g->tests = gb->tests;
    g->testCount = gb->testCount;
    g->actionCount = gb->actionCount;

    g->nameSpace = YSPool_getString(&gb->pool,gb->nameSpace);
    g->tokenPrefix = YSPool_getString(&gb->pool,gb->tokenPrefix);
    g->stype = YSPool_getString(&gb->pool,gb->stype);
    g->prologue = YSPool_getString(&gb->pool,gb->prologue);
    g->dataType = YSPool_getString(&gb->pool,gb->dataType);

    char *p = g->data;
    g->tokenNames = (const char **)p;
    p += sizeof(const char *) * g->tokenCount;

    g->tokenAlias = (const char **)p;
    p += sizeof(const char *) * g->tokenCount;
    
    g->ntNames = (const char **)p;
    p += sizeof(const char *) * g->ntCount;


    g->rules = (YRule *)p;
    p += sizeof(YRule) * g->ruleCount;

    int i,j;
    YSTree_toArray(&gb->tokenEntry,g->tokenNames);
    YSTree_toArray(&gb->ntEntry,g->ntNames);
    YSTree_toArray(&gb->aliasEntry,g->tokenAlias);

    YRawRule *raw;
    YRuleItem *rp = (YRuleItem *)p;
    for(raw = gb->startRule,i = 0;raw != NULL;raw = raw->next,i++){
        YRule *rule = g->rules + i;
        size_t lhs = *YSTree_find(&gb->ntEntry,YSPool_getString(&gb->pool,raw->lhs));
        assert(lhs != 0);

        rule->lhs = lhs - 1;
        rule->index = i;
        rule->length = raw->iLen;
        rule->line = raw->line;
        rule->hasValue = raw->hasValue;
        rule->isGen = raw->isGen;
        rule->stackOffset = raw->stackOffset;
        rule->actionBlock = raw->hasAction ? YSPool_getString(&gb->pool,raw->actionBlock) : NULL;
        rule->rule = rp;

        for(j = 0;j < raw->iLen;j++,rp++){
            YRawRuleItem *ritem = raw->items + j;
            rp->isTerminal = ritem->isTerminal;
            size_t id;
            const char *name = YSPool_getString(&gb->pool,ritem->name);
            if(rp->isTerminal){
                id = *YSTree_find(&gb->tokenEntry,name);
                if(id == 0){
                    fprintf(err,"use of undefined token '%s'",name);
                    goto err;
                }
            }
            else {
                id = *YSTree_find(&gb->ntEntry,name);
                if(id == 0){
                    fprintf(err,"use of undefined non terminal '%s'",name);
                    goto err;
                }
            }
            rp->id = id - 1;
        }
    }
    gb->built = 1;
    return g;
err:
    ya_free(g);
    return NULL;
}


