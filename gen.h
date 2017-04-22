#ifndef __GEN_H__
#define __GEN_H__
#include <stdio.h>
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
    char lah[1];
};

struct _YItemSet{
    YItemSet *prev,*next;
    YGrammar *g;
    int index;
    int len,size;
    size_t itemSize;
    YItem *items;
    int complete;
};

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
