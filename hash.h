#ifndef __YHASH_H__
#define __YHASH_H__

#include <stdlib.h>
#include "ydef.h"

//typedef int (*yhash_comparator)(void *v1,void *v2,void *arg);
typedef unsigned int (*yhash_func)(const void *v,void *arg); 

typedef struct _ybucket{
    int len,size;
    unsigned int hash;
    size_t elemSize;
    int marked;//used when resizing
    char data[1];
}YBucket;

typedef struct _YHashTable{
    YBucket **buckets;
    int bucketCount;
    int entryCount;
    size_t elemSize;
    yhash_func hfunc;
    void *arg;
}YHashTable;

int YHashTable_init(YHashTable *table,yhash_func hf,int size,size_t elemSize);
int YHashTable_free(YHashTable *table);

YBucket **YHashTable_findBucket(YHashTable *table,const void *v);
int YBucket_remove(YBucket *b,int index);
void *YBucket_getItem(YBucket *b,int index);

int YHashTable_add(YHashTable *table,const void *v);
//int YHashTable_resize(YHashTable *table,int newSize);

#endif