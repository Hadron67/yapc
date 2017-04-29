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
#include <string.h>
#include "spool.h"

#define N(s) ((s) == 0 ? NULL : tree->buf + (s))
#define S(s) (pool->buf + s)

int YTree_init(YTree *tree,int size,size_t elemsize,ycomparator comp,void *arg){
    memset(tree,0,sizeof(YTree));
    
    //XXX:align needs to be token into account
    tree->elemSize = elemsize;
    tree->nodeSize = elemsize + sizeof(YTNode);
    tree->size = size;
    tree->len = 0;
    tree->root = -1;
    tree->comp = comp;
    tree->arg = arg;

    tree->buf = (YTNode *)ya_malloc(tree->size * tree->nodeSize);

    return 0;
    
}
int YTree_free(YTree *tree){
    ya_free(tree->buf);
}
int YTree_getLength(YTree *tree){
    return tree->len;
}
YTNode *YTree_getNode(YTree *tree,int p){
    return (YTNode *)((char *)tree->buf + tree->nodeSize * p);
}
int YTree_newNode(YTree *tree,const void *data){
    YTree_prepareNode(tree);
    int p = tree->len++;
    YTNode *node = YTree_getNode(tree,p);
    node->left = node->right = -1;
    memcpy(node->data,data,tree->elemSize);
    return p;
}
int YTree_prepareNode(YTree *tree){
    if(tree->len >= tree->size){
        tree->size *= 2;
        tree->buf = (YTNode *)ya_realloc(tree->buf,tree->nodeSize * tree->size);
    }
    return 0;
}
int *YTree_find(YTree *tree,const void *data){
    int *tp = &tree->root;
    while(*tp != -1){
        int cmp = tree->comp(data,YTree_getNode(tree,*tp)->data,tree->arg);
        if(cmp > 0){
            tp = &YTree_getNode(tree,*tp)->right;
        }
        else if(cmp < 0){
            tp = &YTree_getNode(tree,*tp)->left;
        }
        else {
            break;
        }
    }
    return tp;
}

//-------------------------------------------------------------------
static int YSPool_comp(const void *d1,const void *d2,void *arg){
    YSPool *p = (YSPool *)arg;
    const char *s1 = (const char *)d1;
    ysptr s2 = *(ysptr *)d2;
    return strcmp(s1,p->buf + s2);
}

int YSPool_init(YSPool *pool,size_t bufSize,size_t nBufSize){
    memset(pool,0,sizeof(YSPool));
    pool->bufSize = bufSize;
    pool->buf = (char *)ya_malloc(bufSize * sizeof(char));
    YTree_init(&pool->tree,nBufSize,sizeof(ysptr),YSPool_comp,pool);
    return 0;
}
int YSPool_free(YSPool *pool,char **s){
    YTree_free(&pool->tree);
    if(s != NULL){
        //*s = (char *)ya_realloc(pool->buf,pool->bufLen * sizeof(char));
        *s = pool->buf;
        return 0;
    }
    else {
        ya_free(pool->buf);
        return 0;
    }
}

static int YSPool_newNode(YSPool *pool,const char *s){
    //inserting string

    ysptr sp = pool->bufLen;
    while(*s != 0){
        if(pool->bufLen >= pool->bufSize){
            pool->bufSize *= 2;
            pool->buf = (char *)ya_realloc(pool->buf,pool->bufSize);
        }
        pool->buf[pool->bufLen++] = *s;
        s++;
    }
    if(pool->bufLen >= pool->bufSize){
        pool->bufSize *= 2;
        pool->buf = (char *)ya_realloc(pool->buf,pool->bufSize);
    }
    pool->buf[pool->bufLen++] = 0;

    //inserting node
    return YTree_newNode(&pool->tree,&sp);
}

ysptr YSPool_addString(YSPool *pool,const char *s){
    //find node first.
    YTree_prepareNode(&pool->tree);
    int *node = YTree_find(&pool->tree,(void *)s);
    if(*node == -1){
        *node = YSPool_newNode(pool,s);
    }

    return *(ysptr *)YTree_getNode(&pool->tree,*node)->data;
}

const char *YSPool_getString(YSPool *pool,ysptr sp){
    if(sp < pool->bufLen){
        return pool->buf + sp;
    }
    return NULL;
}

int YSPool_dump(YSPool *pool,FILE *out){
    int i;
    for(i = 0;i < pool->bufLen;i++){
        char c = pool->buf[i];
        if(c == 0)
            fputc('|',out);
        else
            fputc(c,out);
    }
}
