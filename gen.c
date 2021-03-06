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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ydef.h"
#include "gen.h"
#include "hash.h"

static int YItem_comp(const void *d1,const void *d2,void *arg){
    const YItem *i1 = (const YItem *)d1;
    const YItem *i2 = (const YItem *)d2;
    if(i1->rule->index > i2->rule->index){
        return 1;
    }
    else if(i1->rule->index < i2->rule->index){
        return -1;
    }
    else {
        if(i1->marker > i2->marker){
            return 1;
        }
        else if(i1->marker < i2->marker){
            return -1;
        }
        else{
            return 0;
        }
    }
}
static YItemSet *YItemSet_new(YGrammar *g,yheap_t *heap){
    int size = 8;
    size_t itemSize = YGrammar_getTokenSetSize(g) * sizeof(char) + sizeof(YItem);
    YItemSet *set = (YItemSet *)ya_malloc(heap,sizeof(YItemSet));
    //set->items = (YItem *)ya_malloc(size * itemSize);
    set->prev = set->next = NULL;
    set->g = g;
    set->index = -1;
    ///set->len = 0;
    //set->size = size;
    set->itemSize = itemSize;
    set->complete = 0;
    //memset(set->items,0,set->itemSize * size);
    
    YTree_init(&set->itemr,size,itemSize,YItem_comp,NULL,heap);

    return set;
}

static inline YItem *YItemSet_getItem(YItemSet *set,int index){
    //return (YItem *)((char *)set->items + set->itemSize * index);
    return (YItem *)YTree_getNode(&set->itemr,index)->data;
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

static int YItemSet_addItem(YItemSet *set,YRule *rule,int marker,int isKernel,const char *lah,int reset){
    YGrammar *g = set->g;
    YItem temp;
    temp.rule = rule;
    temp.marker = marker;
    ynptr node;
    ynptr *pos = YTree_findEX(&set->itemr,&temp,&node);
    if(*pos == YTNODE_NULL){
        YItem *item = (YItem *)YTree_insertAt(&set->itemr,node,pos);
        memset(item,0,set->itemSize);
        item->rule = rule;
        item->marker = marker;
        item->isKernel = isKernel;
        item->actionType = YACTION_NONE;
        item->changed = 1;
        if(lah != NULL){
            YTokenSet_union(g,item->lah,lah);
        }
        return 1;
    }
    else{
        YItem *item1 = (YItem *)YTree_getNode(&set->itemr,*pos)->data;
        if(lah != NULL){
            int ret = YTokenSet_union(set->g,item1->lah,lah);
            if(reset && ret && YItem_canShift(item1)){
                item1->actionType = YACTION_NONE;
            }
            if(ret){
                item1->changed = 1;
            }
            return ret;
        }
        else {
            return 0;
        }
    }
}

static int YItemSet_canMergeTo(YItemSet *set1,YItemSet *set2){
    YGrammar *g = set1->g;
    int i,j;
    for(i = 0;i < YITEMSET_LEN(set1);i++){
        int found = 0;
        int hasConflict = 0,hasIdentical = 0;
        YItem *i1 = YItemSet_getItem(set1,i);
        for(j = 0;j < YITEMSET_LEN(set2);j++){
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
    for(j = 0;j < YITEMSET_LEN(set2);j++){
        YItem *i2 = YItemSet_getItem(set2,j);
        if(i2->isKernel){
            int found = 0;
            for(i = 0;i < YITEMSET_LEN(set1);i++){
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
    for(i = 0;i < YITEMSET_LEN(src);i++){
        YItem *item = YItemSet_getItem(src,i);
        char ret2 = YItemSet_addItem(dest,item->rule,item->marker,0,item->lah,1);
        ret = ret || ret2;
    }
    return ret;
}

YItemSet *yGenInitialItemSet(YGrammar *g){
    YItemSet *ret = YItemSet_new(g,g->heap);
    YItemSet_addItem(ret,g->rules,0,1,NULL,0);
    YItem *item = YItemSet_getItem(ret,0);
    YTokenSet_add(g,item->lah,1);
    return ret;
}
int YItemSet_free(YItemSet *set,yheap_t *heap){
    YTree_free(&set->itemr);
    ya_free(heap,set);
}
static int YItemSet_closure(YItemSet *set,YFirstSets *fsets){
    YGrammar *g = set->g;
    int changed = 1;
    int ssize = YGrammar_getTokenSetSize(g);
    char tSet[ssize];
    memset(tSet,0,ssize * sizeof(char));
    while(changed){
        changed = 0;
        int i,j;
        for(i = 0;i < YITEMSET_LEN(set);i++){
            YItem *item = YItemSet_getItem(set,i);
            if(item->changed && YItem_canShift(item)){
                YRuleItem *rItem = YItem_getShift(item);
                if(!rItem->isTerminal){
                    YItem_getFollowSet(item,tSet,g,fsets);
                    YRule *rule;
                    for(rule = g->nts[rItem->id].firstRule;rule != NULL;rule = rule->next){
                        if(rule->lhs == rItem->id){
                            int changed2 = YItemSet_addItem(set,rule,0,0,tSet,0);
                            changed = changed || changed2;
                            item = YItemSet_getItem(set,i);
                        }
                    }
                }
                item->changed = 0;
            }
        }
    }

    int i;
    unsigned int ret = 0;
    for(i = 0;i < YITEMSET_LEN(set);i++){
        YItem *item = YItemSet_getItem(set,i);
        if(item->isKernel){
            ret = (ret << 5 + ret) + item->rule->index;
            ret = (ret << 5 + ret) + item->marker;
        }
    }

    set->hash = ret;

    return 0;
}
int YItemSet_markReduce(YItemSet *set){
    int i;
    for(i = 0;i < YITEMSET_LEN(set);i++){
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
    for(i = 0;i < set->itemr.len;i++){
        YItem_dump(YItemSet_getItem(set,i),set->g,showLah,out);
        fprintf(out,"\n");
    }
    return 0;
}

int YItem_dump(YItem *item,YGrammar *g,int showLah,FILE *out){
    
    fprintf(out,"[ %d: %s ->",item->rule->index,g->nts[item->rule->lhs].name);
        
    int i;
    for(i = 0;i < item->rule->length;i++){
        YRuleItem *ritem = item->rule->rule + i;
        if(item->marker == i){
            fprintf(out," .");
        }
        if(ritem->isTerminal){
            fprintf(out," <%s>",g->tokens[ritem->id].name);
        }
        else{
            fprintf(out," %s",g->nts[ritem->id].name);
        }
        
    }
    if(i == item->marker){
        fprintf(out," .");
    }

    if(showLah){
        int first = 1;
        fprintf(out,", { ");
        if(YTokenSet_contains(g,item->lah,0)){
            fprintf(out,"<>");
            first = 0;
        }
        for(i = 0;i < g->tokenCount;i++){
            if(YTokenSet_contains(g,item->lah,i + 1)){
                if(!first){
                    fprintf(out,",");
                }
                else{
                    first = 0;
                }
                fprintf(out,"<%s>",g->tokens[i].name);
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
            break;
        default:
            assert(0);
    }
    
    return 0;
}

int YItemSetList_init(YItemSetList *list){
    memset(list,0,sizeof(YItemSetList));
    list->head.next = &list->tail;
    list->tail.prev = &list->head;
    return 0;
}
int YItemSetList_clear(YItemSetList *list,yheap_t *heap){
    YItemSet *set = list->head.next;
    while(set != &list->tail){
        YItemSet *temp = set->next;
        YItemSet_free(set,heap);
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
    if(set == &list->head || set == &list->tail){
        fprintf(stderr,"attempt to remove the head or tail of a list\n");
        abort();
    }
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

static unsigned int YItemSet_coreHash(const void *s,void *arg){
    const YItemSet *set = *(const YItemSet **)s;
    return set->hash;
}

int yGenItemSets(YGrammar *g,YItemSetList *doneList){
    int index = 0;
    YItemSetList todoList,incList,trash;
    YHashTable htable;
    YFirstSets *fsets = YGrammar_generateFirstSets(g);

    YItemSetList_init(&todoList);
    YItemSetList_init(&incList);
    YItemSetList_init(&trash);
    YHashTable_init(&htable,YItemSet_coreHash,g->ruleCount,sizeof(YItemSet *));
    htable.arg = g;

    YItemSetList_append(&todoList,yGenInitialItemSet(g));

    while(todoList.size != 0 || incList.size != 0){
        YItemSet *comeFrom = NULL;

        //phase 1,generate transition item set from incomplete list.
        if(incList.size != 0){
            YItemSet *set = comeFrom = YItemSetList_poll(&incList);
            int i;
            for(i = 0;i < YITEMSET_LEN(set);i++){
                YItem *item = YItemSet_getItem(set,i);
                if(item->actionType == YACTION_NONE){
                    assert(YItem_canShift(item));
                    YRuleItem *shift = YItem_getShift(item);
                    YItemSet *newSet = trash.size > 0 ? YItemSetList_poll(&trash) : YItemSet_new(g,g->heap);
                    YTree_reInit(&newSet->itemr);
                    newSet->complete = 0;
                    newSet->index = index++;
                    YItemSetList_append(&todoList,newSet);

                    //mark all the symbols in 'set'
                    int j;
                    for(j = 0;j < YITEMSET_LEN(set);j++){
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
            YItemSet *gSet;/*
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
            }*/
            
            
            
            YBucket *bk = *YHashTable_findBucket(&htable,&set);
            if(bk != NULL){
                for(i = 0;i < bk->len;i++){
                    gSet = *(YItemSet **)YBucket_getItem(bk,i);
                    if(YItemSet_canMergeTo(gSet,set)){
                        if(YItemSet_mergeTo(gSet,set)){
                            if(gSet->complete){
                                merged = gSet;
                            }
                        }
                        //fix previous transition actions to merged set
                        if(comeFrom != NULL){
                            int i;
                            for(i = 0;i < YITEMSET_LEN(comeFrom);i++){
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
            }

            //if set is merged with another complete set,
            if(merged != NULL){
                YItemSetList_remove(doneList,merged);
                YItemSetList_append(&incList,merged);
                merged->complete = 0;
            }
            else if(set != NULL){
                YItemSetList_append(&incList,set);
                YHashTable_add(&htable,&set);
            }
        }
    }
    YHashTable_free(&htable);
    YFirstSets_free(fsets);
    YItemSetList_clear(&trash,g->heap);
    YItemSetList_number(doneList);
    return 0;
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
    fprintf(out,YYTAB "token: <%s>\n",g->tokens[c->token].name);
    fprintf(out,YYTAB "used:");
    YItem_dump(c->used,g,0,out);
    fprintf(out,"\n");
    fprintf(out,YYTAB "discarded:");
    YItem_dump(c->discarded,g,0,out);
    fprintf(out,"\n");
    return 0;
}

int YConflicts_print(YArray *c,FILE *out){
    int i;
    for(i = 0;i < c->len;i++){
        YConflict_print((YConflict *)YArray_get(c,i),out);
    }
    return 0;
}
int YParseTable_free(YParseTable *table){
    ya_free(table->g->heap,table);
}
static YItem *yResolveSRConflict(YGrammar *g,YArray *clist,YItemSet *set,YItem *shift,YItem *reduce,int token){
    YTokenDef *td = g->tokens + token;

    //first,try to resolve conflict using operator precedence
    if(td->pr != YP_NONE){
        int ruleP = reduce->rule->prLevel;
        if(ruleP != -1){
            if(ruleP > td->prLevel){
                return reduce;
            }
            else if(ruleP < td->prLevel){
                return shift;
            }
            else{
                if(td->pr == YP_LEFT){
                    return reduce;
                }
                else if(td->pr == YP_RIGHT){
                    return shift;
                }
                else if(td->pr == YP_NONASSOC){
                    return NULL;
                }
                else {  
                    // should not reach here
                    assert(0);
                }
            }
        }
    }
    
    //no precedence available,so do shift and report conflict.
    YConflict *cf = (YConflict *)YArray_push(clist);
    cf->type = YCONFLICT_SR;
    cf->set = set;
    cf->token = token;
    cf->used = shift;
    cf->discarded = reduce;
    return shift;

}

YParseTable *yGenParseTable(YGrammar *g,YItemSetList *list,YArray *clist){
    YParseTable *table = (YParseTable *)ya_malloc(
        g->heap,
        sizeof(YParseTable) +
        sizeof(YItem *) * g->tokenCount * list->size +
        sizeof(YItem *) * g->ntCount * list->size +
        sizeof(int) * g->ntCount * list->size
    );
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
        for(i = 0;i < YITEMSET_LEN(set);i++){
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
                            //YConflictList_addConflict(clist,YCONFLICT_SR,set,rItem->id,item,*cItem);
                            //*cItem = item;
                            *cItem = yResolveSRConflict(g,clist,set,item,*cItem,rItem->id);
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
                                YConflict *cf = (YConflict *)YArray_push(clist);
                                cf->type = YCONFLICT_RR;
                                cf->set = set;
                                cf->token = j;
                                cf->used = item->rule->index > (*cItem)->rule->index ? *cItem : item;
                                cf->discarded = item->rule->index < (*cItem)->rule->index ? *cItem : item;
                                *cItem = cf->used;
                            }
                            else if((*cItem)->actionType == YACTION_SHIFT){
                                //YConflictList_addConflict(clist,YCONFLICT_SR,set,j,*cItem,item);
                                *cItem = yResolveSRConflict(g,clist,set,*cItem,item,j);
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
