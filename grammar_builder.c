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

static int YTokenName_comp(const void *d1,const void *d2,void *arg){
    YGBuilder *gb = (YGBuilder *)arg;
    const char *s1 = (const char *)d1;
    const YRawToken *tk = (const YRawToken *)d2;
    return strcmp(s1,YSPool_getString(&gb->pool,tk->name));
}

static int YTokenAlias_comp(const void *d1,const void *d2,void *arg){
    YGBuilder *gb = (YGBuilder *)arg;
    const char *s1 = (const char *)d1;
    const YRawToken *tk = (const YRawToken *)d2;
    return strcmp(s1,YSPool_getString(&gb->pool,tk->alias));
}

static int Ynt_comp(const void *d1,const void *d2,void *arg){
    YGBuilder *gb = (YGBuilder *)arg;
    const char *s1 = (const char *)d1;
    ysptr s2 = *(const ysptr *)d2;
    return strcmp(s1,YSPool_getString(&gb->pool,s2));
}

int YGBuilder_init(YGBuilder *gb){
    memset(gb,0,sizeof(YGBuilder));
    YSPool_init(&gb->pool,32,32);

    YTree_init(&gb->tokenEntry,32,sizeof(YRawToken),YTokenName_comp,gb);
    YTree_init(&gb->ntEntry,32,sizeof(const char *),Ynt_comp,gb);
    
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
    YTree_free(&gb->tokenEntry);
    YTree_free(&gb->ntEntry);

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
    YTree_prepareNode(&gb->tokenEntry);
    int *node = YTree_find(&gb->tokenEntry,tk);
    if(*node == -1){
        YRawToken rt;
        rt.name = YSPool_addString(&gb->pool,tk);
        rt.alias = YSPool_addString(&gb->pool,alias);
        *node = YTree_newNode(&gb->tokenEntry,&rt);
        return 0;
    }
    else {
        return -1;
    }
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
ysptr YGBuilder_addString(YGBuilder *gb,const char *s){
    return YSPool_addString(&gb->pool,s);
}
static YRawRule *YGBuilder_newRule(YGBuilder *gb){
    YRawRule *raw = (YRawRule *)ya_malloc(sizeof(YRawRule) + gb->ruleSize * sizeof(YRawRuleItem));
    raw->next = NULL;
    raw->iLen = 0;
    raw->iSize = gb->ruleSize;

    return raw;
}
static int YGBuilder_prepareRuleRaw(YGBuilder *gb,const char *lhs,int hasAction,ysptr action,int line){
    if(gb->first){
        gb->first = 0;
        YGBuilder_prepareRule(gb,"(accept)");
        YGBuilder_addRuleItem(gb,lhs,0);
        YGBuilder_commitRule(gb);
    }
    YRawRule *rule = YGBuilder_newRule(gb);
    int *node = YTree_find(&gb->ntEntry,lhs);
    ysptr sp;
    if(*node == -1){
        sp = YSPool_addString(&gb->pool,lhs);
        *node = YTree_newNode(&gb->ntEntry,&sp);
    }
    else {
        sp = *(ysptr *)YTree_getNode(&gb->ntEntry,*node)->data;
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
int hasAction(const char *s){
    int on = 1;
    while(*s){
        if(on){
            if(*s == '$'){
                s++;
                if(*s == '$'){
                    s++;
                    return 1;
                }
            }
            else if(*s == '"'){
                on = 0;
            }
            else {
                s++;
            }
        }
        else{
            if(*s == '"'){
                s++;
                on = 1;
            }
            else if(*s == '\\'){
                s++;
                if(*s){
                    s++;
                }
            }
            else{
                s++;
            }
        }
    }
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
        saved->hasValue = hasAction(action);
        saved->line = line;
    }
    return 0;
}
int YGBuilder_addTestToken(YGBuilder *gb,const char *tname){
    int t = *YTree_find(&gb->tokenEntry,tname);
    if(t == -1){
        return -1;
    }
    if(gb->testLen >= gb->testSize){
        gb->testSize *= 2;
        gb->tests = (int *)ya_realloc(gb->tests,sizeof(int) * gb->testSize);
    }
    gb->tests[gb->testLen++] = t;
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
    YGrammar *g = (YGrammar *)ya_malloc(
        sizeof(YGrammar) + 
        sizeof(YTokenDef) * gb->tokenEntry.len +
        sizeof(const char *) * gb->ntEntry.len +
        sizeof(YRule) * gb->ruleCount + 
        sizeof(YRuleItem) * gb->itemCount
    );
    g->tokenCount = gb->tokenEntry.len;
    g->ntCount = gb->ntEntry.len;
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
    g->tokens = (YTokenDef *)p;
    p += sizeof(YTokenDef) * g->tokenCount;
    
    g->ntNames = (const char **)p;
    p += sizeof(const char *) * g->ntCount;


    g->rules = (YRule *)p;
    p += sizeof(YRule) * g->ruleCount;

    int i,j;
    for(i = 0;i < g->tokenCount;i++){
        YRawToken *rt = (YRawToken *)YTree_getNode(&gb->tokenEntry,i)->data;
        g->tokens[i].name = YSPool_getString(&gb->pool,rt->name);
        g->tokens[i].alias = YSPool_getString(&gb->pool,rt->alias);
    }
    for(i = 0;i < g->ntCount;i++){
        ysptr sp = *(ysptr *)YTree_getNode(&gb->ntEntry,i)->data;
        g->ntNames[i] = YSPool_getString(&gb->pool,sp);
    }

    YRawRule *raw;
    YRuleItem *rp = (YRuleItem *)p;
    for(raw = gb->startRule,i = 0;raw != NULL;raw = raw->next,i++){
        YRule *rule = g->rules + i;
        const char *lhsName = YSPool_getString(&gb->pool,raw->lhs);
        int lhs = *YTree_find(&gb->ntEntry,lhsName);
        assert(lhs != -1);

        rule->lhs = lhs;
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
            int id;
            const char *name = YSPool_getString(&gb->pool,ritem->name);
            if(rp->isTerminal){
                id = *YTree_find(&gb->tokenEntry,name);
                if(id == -1){
                    fprintf(err,"use of undefined token '%s'",name);
                    goto err;
                }
            }
            else {
                id = *YTree_find(&gb->ntEntry,name);
                if(id == -1){
                    fprintf(err,"use of undefined non terminal '%s'",name);
                    goto err;
                }
            }
            rp->id = id;
        }
    }
    gb->built = 1;
    return g;
err:
    ya_free(g);
    return NULL;
}


