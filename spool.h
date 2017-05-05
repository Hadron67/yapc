/*  
    YAPC - Yet Another Parser Compiler - An LR(1) parser generator

    This file contains objects and functions for AVL tree and string 
    pool.


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


#ifndef __SPOOL_H__
#define __SPOOL_H__

#include <stdlib.h>
#include <stdio.h>
#include "ydef.h"
#define YTNODE_NULL (-1)

typedef struct _YSPool YSPool;
typedef struct _YTNode YTNode;
typedef struct _YTree YTree;

typedef long ynptr;

struct _YTNode{
    ynptr left,right,parent;
    ynptr index;
    unsigned int depth;
    char data[1];
};

// used when calling YTree_find,where d1 is the argument data.
typedef int (*ycomparator)(const void *d1,const void *d2,void *arg);

struct _YTree{
    YTNode *buf;
    int len,size;
    size_t elemSize,nodeSize;
    ynptr root;
    ycomparator comp;
    void *arg;
};

int YTree_init(YTree *tree,int size,size_t elemsize,ycomparator comp,void *arg);
int YTree_reInit(YTree *tree);
int YTree_free(YTree *tree);
int YTree_getLength(YTree *tree);
YTNode *YTree_getNode(YTree *tree,ynptr p);
ynptr *YTree_findEX(YTree *tree,const void *data,ynptr *node);
#define YTree_find(tree,data) (YTree_findEX(tree,data,NULL))
void *YTree_insertAt(YTree *tree,ynptr node,ynptr *pos);


//persistent string pool---------------------------------

typedef size_t ysptr;
struct _YSPool{
    char *buf;
    size_t bufLen,bufSize;

    YTree tree;
};

int YSPool_init(YSPool *pool,size_t bufLen,size_t nBufLen);
int YSPool_free(YSPool *pool,char **s);
ysptr YSPool_addString(YSPool *pool,const char *s);
const char *YSPool_getString(YSPool *pool,ysptr sp);
int YSPool_dump(YSPool *pool,FILE *out);

#endif
