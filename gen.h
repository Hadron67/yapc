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

#ifndef __GEN_H__
#define __GEN_H__

#include <stdio.h>

#include "spool.h"
#include "grammar.h"

typedef struct _YItem YItem;
typedef struct _YItemSet YItemSet;

typedef enum {
    YACTION_NONE = 1,
    YACTION_SHIFT,
    YACTION_REDUCE,
}yaction_t;

typedef enum {
    YCONFLICT_RR = 1,
    YCONFLICT_SR
}yconflict_t;

struct _YItem{
    YRule *rule;
    int marker;
    int isKernel;
    yaction_t actionType;
    YItemSet *shift;
    int changed;// used in generating closure
    char lah[1];
};

struct _YItemSet{
    YItemSet *prev,*next;
    YGrammar *g;
    int index;
    int pact;//used in generating parsing table,for compressing.
    //int len,size;
    size_t itemSize;
    //YItem *items;
    YTree itemr;
    unsigned int hash;
    int complete;
};

#define YITEMSET_LEN(s) ((s)->itemr.len)

typedef struct _YItemSetList{
    YItemSet head,tail;
    int size;
}YItemSetList;


typedef struct _YParseTable{
    YGrammar *g;
    int stateCount;
    YItem **shift;
    YItem **gotot;
    int *action;

    char data[1];
}YParseTable;

typedef struct _YConflict{
    yconflict_t type;
    int token;
    YItemSet *set;
    YItem *used,*discarded;
}YConflict;

typedef struct _YConflictList{
    int len,size;
    YConflict *conflicts;
}YConflictList;

YItemSet *yGenInitialItemSet(YGrammar *g);
int YItemSet_free(YItemSet *set);
int YItemSet_dump(YItemSet *set,int showLah,FILE *out);
int YItem_dump(YItem *item,YGrammar *g,int showLah,FILE *out);

int YItemSetList_init(YItemSetList *list);
int YItemSetList_clear(YItemSetList *list);
int YItemSetList_append(YItemSetList *list,YItemSet *set);
int YItemSetList_remove(YItemSetList *list,YItemSet *set);
int YItemSetList_number(YItemSetList *list);
int YItemSetList_dump(YItemSetList *list,int showLah,FILE *out);
YItemSet *YItemSetList_poll(YItemSetList *list);

int YConflictList_init(YConflictList *list,int size);
int YConflictList_free(YConflictList *list);
int YConflictList_print(YConflictList *list,FILE *out);

int YParseTable_free(YParseTable *table);

int yGenItemSets(YGrammar *g,YItemSetList *out);
YParseTable *yGenParseTable(YGrammar *g,YItemSetList *list,YConflictList *clist);



#endif
