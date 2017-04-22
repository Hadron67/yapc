/*  
    YAPC - Yet Another Parser Compiler - An LR(1) parser generater

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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ydef.h"
#include "gen.h"

#define BITS 8

static YItemSet *YItemSet_new(YGrammar *g){
    int size = 8;
    size_t itemSize = YGrammar_getTokenSetSize(g) * sizeof(char) + sizeof(YItem);
    YItemSet *set = (YItemSet *)ya_malloc(sizeof(YItemSet));
    set->items = (YItem *)ya_malloc(size * itemSize);
    set->prev = set->next = NULL;
    set->g = g;
    set->index = -1;
    set->len = 0;
    set->size = size;
    set->itemSize = itemSize;
    set->complete = 0;
    memset(set->items,0,set->itemSize * size);
    
    return set;
}

static YItem *YItemSet_getItem(YItemSet *set,int index){
    return (YItem *)((char *)set->items + set->itemSize * index);
}

static YItem *YItemSet_addItemRaw(YItemSet *set1){
    if(set1->len >= set1->size){
        set1->size *= 2;
        //set1 = *set = (YItemSet *)ya_realloc(set1,sizeof(YItemSet) + set1->size * set1->itemSize);
        set1->items = (YItem *)ya_realloc(set1->items,set1->size * set1->itemSize);
    }
    YItem *item = YItemSet_getItem(set1,set1->len++);
    memset(item,0,set1->itemSize);
    item->actionType = YACTION_NONE;
    return item;
}

static YRuleItem *YItem_getShift(YItem *item){
    return item->rule->rule + item->marker;
}
static int YItem_canShift(YItem *item){
    return item->marker < item->rule->length;
}
static int YItem_hasRRConflictWith(YItem *i1,YItem *i2,YGrammar *g){
    return i1->actionType == YACTION_REDUCE && i2->actionType == YACTION_REDUCE 
    && i1->rule != i2->rule
    && YTokenSet_hasIntersection(g,i1->lah,i2->lah);
}

static int YItem_getFollowSet(YItem *item,char *set,YGrammar *g,YFirstSets *fsets){
    memset(set,0,YGrammar_getTokenSetSize(g) *  sizeof(char));
    int i;
    for(i = item->marker + 1;i < item->rule->length;i++){
        YRuleItem *rItem = item->rule->rule + i;
        if(rItem->isTerminal){
            YTokenSet_add(g,set,rItem->id + 1);
            break;
        }
        else {
            char *set1 = YFirstSets_getSet(fsets,rItem->id);
            YTokenSet_union(g,set,set1);
            YTokenSet_removeToken(g,set,0);
            if(!YTokenSet_contains(g,set1,0)){
                break;
            }
        }
    }
    if(i == item->rule->length){
        YTokenSet_union(g,set,item->lah);
    }

    return 0;
}

static int YItemSet_addItem(YItemSet *set,YRule *rule,int marker,int isKernel,char *lah,int reset){
    YGrammar *g = set->g;
    // check for duplications
    int i;
    for(i = 0;i < set->len;i++){
        YItem *item1 = YItemSet_getItem(set,i);
        if(item1->rule == rule && item1->marker == marker){
            int ret = YTokenSet_union(set->g,item1->lah,lah);
            if(reset && ret && YItem_canShift(item1)){
                item1->actionType = YACTION_NONE;
            }
            return ret;
        }
    }
    
    YItem *item1 = YItemSet_addItemRaw(set);
    item1->rule = rule;
    item1->marker = marker;
    item1->isKernel = isKernel;
    
    YTokenSet_union(set->g,item1->lah,lah);
    
    return 1;
}

static int YItemSet_canMergeTo(YItemSet *set1,YItemSet *set2){
    YGrammar *g = set1->g;
    int i,j;
    for(i = 0;i < set1->len;i++){
        int found = 0;
        int hasConflict = 0,hasIdentical = 0;
        YItem *i1 = YItemSet_getItem(set1,i);
        for(j = 0;j < set2->len;j++){
            YItem *i2 = YItemSet_getItem(set2,j);
            if(i1->rule == i2->rule && i1->marker == i2->marker){
                hasIdentical = YTokenSet_isIdentical(i1->lah,i2->lah,g);
                found = i1->isKernel && i2->isKernel;
            }
            hasConflict = YItem_hasRRConflictWith(i1,i2,g);
        }
        if(i1->isKernel && !found || hasConflict && !hasIdentical){
            return 0;
        }
    }
    for(j = 0;j < set2->len;j++){
        YItem *i2 = YItemSet_getItem(set2,j);
        if(i2->isKernel){
            int found = 0;
            for(i = 0;i < set1->len;i++){
                YItem *i1 = YItemSet_getItem(set1,i);
                if(i1->isKernel && i2->isKernel && i1->rule == i2->rule && i1->marker == i2->marker){
                    found = 1;
                }
            }
            if(!found){
                return 0;
            }
        }
    }
    return 1;
}
static int YItemSet_mergeTo(YItemSet *dest,YItemSet *src){
    int i;
    int ret = 0;
    for(i = 0;i < src->len;i++){
        YItem *item = YItemSet_getItem(src,i);
        ret = ret || YItemSet_addItem(dest,item->rule,item->marker,0,item->lah,1);
    }
    return ret;
}

YItemSet *yGenInitialItemSet(YGrammar *g){
    YItemSet *ret = YItemSet_new(g);
    YItem *item = YItemSet_addItemRaw(ret);
    item->rule = g->rules;
    item->marker = 0;
    item->isKernel = 1;
    YTokenSet_add(g,item->lah,1);
    return ret;
}
int YItemSet_free(YItemSet *set){
    ya_free(set->items);
    ya_free(set);
}
static int YItemSet_closure(YItemSet *set,YFirstSets *fsets){
    YGrammar *g = set->g;
    int changed = 1;
    char tSet[YGrammar_getTokenSetSize(g)];
    while(changed){
        changed = 0;
        int i,j;
        for(i = 0;i < set->len;i++){
            YItem *item = YItemSet_getItem(set,i);
            if(YItem_canShift(item)){
                YRuleItem *rItem = YItem_getShift(item);
                if(!rItem->isTerminal){
                    YItem_getFollowSet(item,tSet,g,fsets);
                    for(j = 0;j < g->ruleCount;j++){
                        YRule *rule = g->rules + j;
                        if(rule->lhs == rItem->id){
                            changed = changed || YItemSet_addItem(set,rule,0,0,tSet,0);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
int YItemSet_markReduce(YItemSet *set){
    int i;
    for(i = 0;i < set->len;i++){
        YItem *item = YItemSet_getItem(set,i);
        if(!YItem_canShift(item)){
            item->actionType = YACTION_REDUCE;
        }
    }
    return 0;
}

int YItemSet_dump(YItemSet *set,int showLah,FILE *out){
    int i;
    fprintf(out,"i%d\n",set->index);
    for(i = 0;i < set->len;i++){
        YItem_dump(YItemSet_getItem(set,i),set->g,showLah,out);
        fprintf(out,"\n");
    }
    return 0;
}

int YItem_dump(YItem *item,YGrammar *g,int showLah,FILE *out){
    
    fprintf(out,"[ %d: %s ->",item->rule->index,g->ntNames[item->rule->lhs]);
        
    int i;
    for(i = 0;i < item->rule->length;i++){
        YRuleItem *ritem = item->rule->rule + i;
        if(item->marker == i){
            fprintf(out," .");
        }
        if(ritem->isTerminal){
            fprintf(out," <%s>",g->tokenNames[ritem->id]);
        }
        else{
            fprintf(out," %s",g->ntNames[ritem->id]);
        }
        
    }
    if(i == item->marker){
        fprintf(out," .");
    }

    if(showLah){
        fprintf(out,", { ");
        if(YTokenSet_contains(g,item->lah,0)){
            fprintf(out,"<>,");
        }
        for(i = 0;i < g->tokenCount;i++){
            if(YTokenSet_contains(g,item->lah,i + 1)){
                fprintf(out,"<%s>,",g->tokenNames[i]);
            }
        }
        fprintf(out," } ]");
    }
    else {
        fprintf(out," ]");
    }
    if(item->isKernel){
        fprintf(out,"*");
    }
    
    switch(item->actionType){
        case YACTION_NONE:
            fprintf(out," (-)");
            break;
        case YACTION_SHIFT:
            fprintf(out," (s%d)",item->shift->index);
            break;
        case YACTION_REDUCE:
            fprintf(out," (r)");
    }
    
    return 0;
}

int YItemSetList_init(YItemSetList *list){
    memset(list,0,sizeof(YItemSetList));
    list->head.next = &list->tail;
    list->tail.prev = &list->head;
    return 0;
}
int YItemSetList_clear(YItemSetList *list){
    YItemSet *set = list->head.next;
    while(set != &list->tail){
        YItemSet *temp = set->next;
        YItemSet_free(set);
        set = temp;
    }
    list->head.next = &list->tail;
    list->tail.prev = &list->head;
    list->size = 0;
    return 0;
}
int YItemSetList_append(YItemSetList *list,YItemSet *set){
    set->prev = list->tail.prev;
    set->next = &list->tail;
    list->tail.prev->next = set;
    list->tail.prev = set;
    list->size++;
    return 0;
}
int YItemSetList_remove(YItemSetList *list,YItemSet *set){
    set->prev->next = set->next;
    set->next->prev = set->prev;
    set->prev = set->next = NULL;
    list->size--;
    return 0;
}
int YItemSetList_dump(YItemSetList *list,int showLah,FILE *out){
    YItemSet *set;
    for(set = list->head.next;set != &list->tail;set = set->next){
        YItemSet_dump(set,showLah,out);
        fprintf(out,"\n");
    }
}
YItemSet *YItemSetList_poll(YItemSetList *list){
    if(list->size == 0){
        return NULL;
    }
    YItemSet *ret = list->tail.prev;
    YItemSetList_remove(list,ret);
    return ret;
}
int YItemSetList_number(YItemSetList *list){
    YItemSet *set;
    int i;
    for(i = 0,set = list->head.next;set != &list->tail;i++,set = set->next){
        set->index = i;
    }
    return 0;
}

int yGenItemSets(YGrammar *g,YItemSetList *doneList){
    int index = 0;
    YItemSetList todoList,incList,trash;
    YFirstSets *fsets = YGrammar_generateFirstSets(g);

    YItemSetList_init(&todoList);
    YItemSetList_init(&incList);
    YItemSetList_init(&trash);

    YItemSetList_append(&todoList,yGenInitialItemSet(g));

    while(todoList.size != 0 || incList.size != 0){
        YItemSet *comeFrom = NULL;

        //phase 1,generate transition item set from incomplete list.
        if(incList.size != 0){
            YItemSet *set = comeFrom = YItemSetList_poll(&incList);
            int i;
            for(i = 0;i < set->len;i++){
                YItem *item = YItemSet_getItem(set,i);
                if(item->actionType == YACTION_NONE){
                    assert(YItem_canShift(item));
                    YRuleItem *shift = YItem_getShift(item);
                    YItemSet *newSet = trash.size > 0 ? YItemSetList_poll(&trash) : YItemSet_new(g);
                    newSet->len = 0;
                    newSet->complete = 0;
                    newSet->index = index++;
                    YItemSetList_append(&todoList,newSet);

                    //mark all the symbols in 'set'
                    int j;
                    for(j = 0;j < set->len;j++){
                        YItem *item1 = YItemSet_getItem(set,j);
                        YRuleItem *rItem = YItem_getShift(item1);
                        if(YItem_canShift(item1) && rItem->id == shift->id && rItem->isTerminal == shift->isTerminal){
                            item1->actionType = YACTION_SHIFT;
                            item1->shift = newSet;
                            YItemSet_addItem(newSet,item1->rule,item1->marker + 1,1,item1->lah,0);
                        }
                    }
                }
            }
            set->complete = 1;
            YItemSetList_append(doneList,set);
        }

        //phase 2
        while(todoList.size != 0){
            YItemSet *set = YItemSetList_poll(&todoList);
            YItemSet_closure(set,fsets);
            YItemSet_markReduce(set);
            YItemSet *merged = NULL;
            
            //find possible merges
            int i;
            YItemSet *gSet;
            for(gSet = doneList->head.next;gSet != &doneList->tail;gSet = gSet->next){
                if(YItemSet_canMergeTo(gSet,set)){
                    if(YItemSet_mergeTo(gSet,set)){
                        merged = gSet;
                    }

                    //fix previous transition actions to merged set
                    if(comeFrom != NULL){
                        int i;
                        for(i = 0;i < comeFrom->len;i++){
                            YItem *sItem = YItemSet_getItem(comeFrom,i);
                            if(sItem->actionType == YACTION_SHIFT && sItem->shift == set){
                                sItem->shift = gSet;
                            }
                        }
                    }
                    YItemSetList_append(&trash,set);
                    //YItemSet_free(set);
                    set = NULL;
                    break;
                }
            }
            //find possible merge in incomplete list
            if(set != NULL){
                int i;
                YItemSet *gSet;
                for(gSet = incList.head.next;gSet != &incList.tail;gSet = gSet->next){
                    if(YItemSet_canMergeTo(gSet,set)){
                        YItemSet_mergeTo(gSet,set);
                        if(comeFrom != NULL){
                            int i;
                            for(i = 0;i < comeFrom->len;i++){
                                YItem *sItem = YItemSet_getItem(comeFrom,i);
                                if(sItem->actionType == YACTION_SHIFT && sItem->shift == set){
                                    sItem->shift = gSet;
                                }
                            }
                        }
                        YItemSetList_append(&trash,set);
                        set = NULL;
                        break;
                    }
                }
            }
            //if set is merged with another set,
            if(merged != NULL){
                YItemSetList_remove(doneList,merged);
                YItemSetList_append(&incList,merged);
                merged->complete = 0;
            }
            else if(set != NULL){
                YItemSetList_append(&incList,set);
            }
        }
    }
    YFirstSets_free(fsets);
    YItemSetList_clear(&trash);
    YItemSetList_number(doneList);
    return 0;
}
int YConflictList_init(YConflictList *list,int size){
    list->len = 0;
    list->size = size;
    list->conflicts = (YConflict *)ya_malloc(sizeof(YConflict) * size);
    return 0;
}
static int YConflictList_addConflict(YConflictList *list,yconflict_t type,YItemSet *set,YItem *used,YItem *discarded){
    if(list->len >= list->size){
        list->size *= 2;
        list->conflicts = (YConflict *)ya_realloc(list->conflicts,sizeof(YConflict) * list->size);
    }
    YConflict *c = list->conflicts + list->len++;
    c->set = set;
    c->used = used;
    c->type = type;
    c->discarded = discarded;

    return 0;
}
int YConflictList_free(YConflictList *list){
    ya_free(list->conflicts);
}
static int YConflict_print(YConflict *c,FILE *out){
    YGrammar *g = c->set->g;
    if(c->type == YCONFLICT_RR){
        fprintf(out,"state %d,reduce/reduce conflict:\n",c->set->index);
    }
    else if(c->type == YCONFLICT_SR){
        fprintf(out,"state %d,shift/reduce conflict:\n",c->set->index);
    }
    else {
        abort();
    }
    fprintf(out,"  used:");
    YItem_dump(c->used,g,1,out);
    fprintf(out,"\n");
    fprintf(out,"  discarded:");
    YItem_dump(c->discarded,g,1,out);
    fprintf(out,"\n");
    return 0;
}
int YConflictList_print(YConflictList *list,FILE *out){
    int i;
    for(i = 0;i < list->len;i++){
        YConflict_print(list->conflicts + i,out);
        fprintf(out,"\n");
    }
    return 0;
}
int YParseTable_free(YParseTable *table){
    ya_free(table);
}
YParseTable *yGenParseTable(YGrammar *g,YItemSetList *list,YConflictList *clist){
    YParseTable *table = (YParseTable *)ya_malloc(
        sizeof(YParseTable) +
        sizeof(YItem *) * g->tokenCount * list->size +
        sizeof(YItem *) * g->ntCount * list->size +
        sizeof(int) * g->ntCount * list->size
    );
    //YConflictList_init(clist,4);
    table->stateCount = list->size;
    char *p =  table->data;

    table->shift = (YItem **)p;
    p += sizeof(YItem *) * g->tokenCount * list->size;

    table->gotot = (YItem **)p;
    p += sizeof(YItem *) * g->ntCount * list->size;

    table->action = (int *)p;
    p += sizeof(int *) * g->ntCount * list->size;

    table->g = g;

    int i;
    for(i = 0;i < g->tokenCount * list->size;i++){
        table->shift[i] = NULL;
    }
    for(i = 0;i < g->ntCount * list->size;i++){
        table->gotot[i] = NULL;
    }
    YItemSet *set;
    for(set = list->head.next;set != &list->tail;set = set->next){
        int j,k;
        for(i = 0;i < set->len;i++){
            YItem *item = YItemSet_getItem(set,i);
            if(item->actionType == YACTION_SHIFT){
                YRuleItem *rItem = YItem_getShift(item);
                if(rItem->isTerminal){
                    int index = set->index * g->tokenCount + rItem->id;
                    YItem **cItem = table->shift + index;
                    if(*cItem != NULL){
                        if((*cItem)->actionType == YACTION_SHIFT){
                            assert((*cItem)->shift == item->shift);
                        }
                        else if((*cItem)->actionType == YACTION_REDUCE){
                            //now we've got a shift-reduce conflict.
                            //report it,and do shift
                            assert((*cItem)->actionType == YACTION_REDUCE);
                            YConflictList_addConflict(clist,YCONFLICT_SR,set,item,*cItem);
                            *cItem = item;
                        }
                    }
                    else {
                        *cItem = item;
                    }
                }
                else {
                    int index = set->index * g->ntCount + rItem->id;
                    table->gotot[index] = item;
                }
            }
            else if(item->actionType == YACTION_REDUCE){
                for(j = 0;j < g->tokenCount;j++){
                    if(YTokenSet_contains(g,item->lah,j + 1)){
                        int index = set->index * g->tokenCount + j;
                        YItem **cItem = table->shift + index;
                        if(*cItem != NULL){
                            if((*cItem)->actionType == YACTION_REDUCE){
                                //we've got a reduce-reduce conflict.
                                YItem *taken = item->rule->index > (*cItem)->rule->index ? *cItem : item;
                                YItem *discarded = item->rule->index < (*cItem)->rule->index ? *cItem : item;
                                YConflictList_addConflict(clist,YCONFLICT_RR,set,taken,discarded);
                                *cItem = taken;
                            }
                            else if((*cItem)->actionType == YACTION_SHIFT){
                                YConflictList_addConflict(clist,YCONFLICT_SR,set,*cItem,item);
                            }
                        }
                        else {
                            *cItem = item;
                        }
                    }
                }
            }
        }   
    }

    return table;
}
