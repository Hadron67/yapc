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
#include "array.h"

typedef struct _YTStackItem{
    int isTerminal;
    int id;
}YTStackItem;

typedef struct _YTest{
    yheap_t *heap;
    YGrammar *g;
    YParseTable *table;
    FILE *out;
    YArray stack;
    YArray state;
    
    int *test;
    int len,p;

}YTest;

static int YTest_init(YTest *t){
    YArray_init(&t->stack,sizeof(YTStackItem),16,t->heap);
    YArray_init(&t->state,sizeof(int),16,t->heap);
    return 0;
}

static int YTest_free(YTest *t){
    YArray_free(&t->stack,NULL);
    YArray_free(&t->state,NULL);
    return 0;
}

static int YTest_pop(YTest *t,int count){
    while(count --> 0){
        YArray_pop(&t->state);
    }
}

static int YTest_dump(YTest *t){
    int i;
    for(i = 0;i < t->stack.len;i++){
        YTStackItem *item = (YTStackItem *)YArray_get(&t->stack,i);
        if(item->isTerminal){
            fprintf(t->out,"<%s> ",t->g->tokens[item->id].name);
        }
        else {
            fprintf(t->out,"%s ",t->g->nts[item->id].name);
        }
    }
    fprintf(t->out,"| ");
    for(i = t->p;i < t->len;i++){
        fprintf(t->out,"<%s> ",t->g->tokens[t->test[i]].name);
    }
    fprintf(t->out,"[");
    for(i = 0;i < t->state.len;i++){
        fprintf(t->out,"%d ",*(int *)YArray_get(&t->state,i));
    }
    fprintf(t->out,"]\n");
    return 0;
}

static int yRunTest(YTest *t){
    t->p = 0;
    t->stack.len = 0;
    t->state.len = 0;
    *(int *)YArray_push(&t->state) = 0;
    while(1){
        YTest_dump(t);
        int token = t->p >= t->len ? 0 : t->test[t->p];
        int state = *(int *)YArray_get(&t->state,t->state.len - 1);
        int index = state * t->g->tokenCount + token;
        YItem *item = t->table->shift[index];
        if(item == NULL){
            fprintf(t->out,"syntax error!unexpected token '%s' (%s)\n\n",t->g->tokens[token].name,t->g->tokens[token].alias);
            return -1;
        }
        else if(item->actionType == YACTION_SHIFT){
            //do a shift
            YTStackItem *si = (YTStackItem *)YArray_push(&t->stack);
            si->id = token;
            si->isTerminal = 1;
            *(int *)YArray_push(&t->state) = item->shift->index;
            t->p++;
        }
        else if(item->actionType == YACTION_REDUCE){
            //do a reduction
            if(item->rule->lhs == 0){
                fprintf(t->out,"accepted!\n\n");
                break;//accept
            }
            int i = item->rule->length;
            while(i --> 0){
                YArray_pop(&t->state);
                YArray_pop(&t->stack);
            }
            YTStackItem *si = (YTStackItem *)YArray_push(&t->stack);
            si->id = item->rule->lhs;
            si->isTerminal = 0;
            state = *(int *)YArray_get(&t->state,t->state.len - 1);
            index = state * t->g->ntCount + item->rule->lhs;
            YItem *i2 = t->table->gotot[index];
            *(int *)YArray_push(&t->state) = i2->shift->index;
        }
    }
}

int yRunTests(YGrammar *g,YParseTable *table,FILE *out){
    YTest test;
    // XXX: replace with a valid heap
    test.heap = NULL;
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