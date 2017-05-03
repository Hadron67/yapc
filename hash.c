#include <string.h>
#include "hash.h"

int YHashTable_init(YHashTable *table,yhash_func hf,int size,size_t elemSize){
    memset(table,0,sizeof(YHashTable));
    table->hfunc = hf;
    table->elemSize = elemSize;
    table->bucketCount = size;
    table->buckets = (YBucket **)ya_malloc(sizeof(YBucket *) * size);
    memset(table->buckets,0,sizeof(YBucket *) * size);
    return 0;
}
int YHashTable_free(YHashTable *table){
    int i;
    for(i = 0;i < table->bucketCount;i++){
        if(table->buckets[i] != NULL){
            ya_free(table->buckets[i]);
        }
    }
    ya_free(table->buckets);
    return 0;
}
static YBucket *YBucket_new(size_t elemSize){
    const int size = 16;
    YBucket *ret = (YBucket *)ya_malloc(sizeof(YBucket) + elemSize * size);
    ret->size = size;
    ret->len = 0;
    ret->elemSize = elemSize;
    ret->marked = 0;
    return ret;
}
static int YBucket_add(YBucket **b,const void *data){
    YBucket *bk = *b;
    int i;
    if(bk->len >= bk->size){
        bk->size *= 2;
        bk = (YBucket *)ya_realloc(bk,sizeof(YBucket) + bk->elemSize * bk->size);
    }
    memcpy(bk->data + bk->elemSize * bk->len++,data,bk->elemSize);
    *b = bk;
    return 0;
}
YBucket **YHashTable_findBucket(YHashTable *table,const void *v){
    unsigned int hash = table->hfunc(v,table->arg) % table->bucketCount;
    return table->buckets + hash;
}
int YBucket_remove(YBucket *b,int index){
    for(;index < b->len;index++){
        memcpy(b->data + index,b->data + index + 1,b->elemSize);
    }
    return 0;
}
void *YBucket_getItem(YBucket *b,int index){
    return b->data + b->elemSize * index;
}
int YHashTable_add(YHashTable *table,const void *v){
    unsigned int hash = table->hfunc(v,table->arg);
    YBucket **b = table->buckets + hash % table->bucketCount;
    if(*b == NULL){
        *b = YBucket_new(table->elemSize);
        (*b)->hash = hash;
    }
    table->entryCount++;
    return YBucket_add(b,v);
}
