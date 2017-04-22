#ifndef __SPOOL_H__
#define __SPOOL_H__

#include <stdlib.h>
#include <stdio.h>
#include "ydef.h"

typedef struct _YSPool YSPool;
typedef struct _YSNode YSNode;
typedef struct _YSTree YSTree;

struct _YSNode{
    size_t left,right;
    size_t sptr;
    int index;
};

struct _YSTree{
    YSPool *sbuf;
    YSNode *buf;
    size_t len,size;
    size_t root;
};

int YSTree_init(YSTree *tree,size_t size,YSPool *pool);
int YSTree_free(YSTree *tree);
YSNode *YSTree_getNode(YSTree *tree,size_t p);
size_t YSTree_newNode(YSTree *tree,size_t s);
int YSTree_prepareNode(YSTree *tree);
size_t *YSTree_find(YSTree *tree,const char *s);
int YSTree_toArray(YSTree *tree,const char **sa);

//persistent string pool---------------------------------

struct _YSPool{
    char *buf;
    size_t bufLen,bufSize;

    YSTree tree;
};

int YSPool_init(YSPool *pool,size_t bufLen,size_t nBufLen);
int YSPool_free(YSPool *pool,char **s);
size_t YSPool_addString(YSPool *pool,const char *s);
const char *YSPool_getString(YSPool *pool,size_t sp);
int YSPool_dump(YSPool *pool,FILE *out);

#endif
