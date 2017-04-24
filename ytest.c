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

#include "ydef.h"
#include <stdlib.h>
#include <string.h>
#include "ytest.h"

typedef struct _YTStackItem{
    int isTerminal;
    int id;
}YTStackItem;

typedef struct _YTStack{
    int len,size;
    YTStackItem *items;
}YTStack; 

typedef struct _YTest{
    YGrammar *g;
    YParseTable *table;
    FILE *out;
    YTStack stack;
    int *test;
    int len,p;

    int *state;
    int sLen,sSize;
}YTest;

static int YTStack_init(YTStack *s){
    s->len = 0;
    s->size = 16;
    s->items = (YTStackItem *)ya_malloc(sizeof(YTStack) * s->size);
}

static int YTStack_free(YTStack *s){
    ya_free(s->items);
}

static int YTStack_push(YTStack *s,int isTerminal,int id){
    if(s->len >= s->size){
        s->size *= 2;
        s->items = (YTStackItem *)ya_realloc(s->items,sizeof(YTStack) * s->size);
    }
    YTStackItem *i = s->items + s->len++;
    i->isTerminal = isTerminal;
    i->id = id;
}

static int YTStack_pop(YTStack *s,int count){
    while(count --> 0){
        s->len--;
    }
    return 0;
}

static int YTest_init(YTest *t){
    YTStack_init(&t->stack);
    t->sLen = 0;
    t->sSize = 16;
    t->state = (int *)ya_malloc(sizeof(int) * t->sSize);
    return 0;
}

static int YTest_free(YTest *t){
    YTStack_free(&t->stack);
    ya_free(t->state);
}

static int YTest_pushS(YTest *t,int s){
    if(t->sLen >= t->sSize){
        t->sLen *= 2;
        t->state = (int *)ya_realloc(t->state,sizeof(int) * t->sSize);
    }
    t->state[t->sLen++] = s;
    return 0;
}

static int YTest_pop(YTest *t,int count){
    while(count --> 0){
        t->sLen--;
    }
}

static int YTest_dump(YTest *t){
    int i;
    for(i = 0;i < t->stack.len;i++){
        YTStackItem *item = t->stack.items + i;
        if(item->isTerminal){
            fprintf(t->out,"<%s> ",t->g->tokenNames[item->id]);
        }
        else {
            fprintf(t->out,"%s ",t->g->ntNames[item->id]);
        }
    }
    fprintf(t->out,"| ");
    for(i = t->p;i < t->len;i++){
        fprintf(t->out,"<%s> ",t->g->tokenNames[t->test[i]]);
    }
    fprintf(t->out,"[");
    for(i = 0;i < t->sLen;i++){
        fprintf(t->out,"%d ",t->state[i]);
    }
    fprintf(t->out,"]\n");
    return 0;
}

static int yRunTest(YTest *t){
    t->sLen = t->p = 0;
    t->stack.len = 0;
    YTest_pushS(t,0);
    while(1){
        YTest_dump(t);
        int token = t->p >= t->len ? 0 : t->test[t->p];
        int state = t->state[t->sLen - 1];
        int index = state * t->g->tokenCount + token;
        YItem *item = t->table->shift[index];
        if(item == NULL){
            fprintf(t->out,"syntax error!unexpected token '%s' (%s)\n\n",t->g->tokenNames[token],t->g->tokenAlias[token]);
            return -1;
        }
        else if(item->actionType == YACTION_SHIFT){
            //do a shift
            YTStack_push(&t->stack,1,token);
            YTest_pushS(t,item->shift->index);
            t->p++;
        }
        else if(item->actionType == YACTION_REDUCE){
            //do a reduction
            if(item->rule->lhs == 0){
                fprintf(t->out,"accepted!\n\n");
                break;//accept
            }
            YTest_pop(t,item->rule->length);
            YTStack_pop(&t->stack,item->rule->length);
            YTStack_push(&t->stack,0,item->rule->lhs);
            state = t->state[t->sLen - 1];
            index = state * t->g->ntCount + item->rule->lhs;
            YItem *i2 = t->table->gotot[index];
            YTest_pushS(t,i2->shift->index);
        }
    }
}

int yRunTests(YGrammar *g,YParseTable *table,FILE *out){
    YTest test;
    YTest_init(&test);
    test.g = g;
    test.out = out;
    test.table = table;
    int i;
    int *t = g->tests;
    for(i = 0;i < g->testCount;i++){
        test.len = *t;
        test.test = t + 1;
        fprintf(out,"test %d:\n",i + 1);
        yRunTest(&test);
        t += *t + 1;
    }

    YTest_free(&test);
}