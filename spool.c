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
#include <string.h>
#include "spool.h"

#define N(s) ((s) == 0 ? NULL : tree->buf + (s))
#define S(s) (pool->buf + s)

int YSTree_init(YSTree *tree,size_t size,YSPool *pool){
    memset(tree,0,sizeof(YSNode));
    tree->size = size + 1;
    tree->sbuf = pool;
    tree->len = 1;

    tree->buf = (YSNode *)ya_malloc(tree->size * sizeof(YSNode));

    return 0;
}
int YSTree_free(YSTree *tree){
    ya_free(tree->buf);
    return 0;
}
int YSTree_prepareNode(YSTree *tree){
    if(tree->len >= tree->size){
        tree->size *= 2;
        tree->buf = (YSNode *)ya_realloc(tree->buf,sizeof(YSNode) * tree->size);
    }
    return 0;
}
size_t YSTree_newNode(YSTree *tree,size_t s){
    YSTree_prepareNode(tree);
    size_t p = tree->len;
    YSNode *ret = tree->buf + tree->len++;
    ret->left = ret->right = 0;
    ret->sptr = s;
    ret->index = tree->len - 2;
    return p;
}
YSNode *YSTree_getNode(YSTree *tree,size_t p){
    if(p < tree->len){
        return tree->buf + p;
    }
    return NULL;
}
size_t *YSTree_find(YSTree *tree,const char *s){
    size_t *tp = &tree->root;
    while(*tp != 0){
        int cmp = strcmp(s,tree->sbuf->buf + tree->buf[*tp].sptr);
        if(cmp > 0){
            tp = &tree->buf[*tp].right;
        }
        else if(cmp < 0){
            tp = &tree->buf[*tp].left;
        }
        else {
            break;
        }
    }
    assert(tp != &tree->buf[*tp].right && tp != &tree->buf[*tp].left);
    return tp;
}
int YSTree_toArray(YSTree *tree,const char **sa){
    int i;
    const char *sbuf = tree->sbuf->buf;
    for(i = 1;i < tree->len;i++){
        sa[i - 1] = sbuf + tree->buf[i].sptr;
    }
    return 0;
}

//-------------------------------------------------------------------

int YSPool_init(YSPool *pool,size_t bufSize,size_t nBufSize){
    memset(pool,0,sizeof(YSPool));
    pool->bufSize = bufSize;
    pool->buf = (char *)ya_malloc(bufSize * sizeof(char));
    YSTree_init(&pool->tree,nBufSize,pool);

    return 0;
}
int YSPool_free(YSPool *pool,char **s){
    YSTree_free(&pool->tree);
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

static ssize_t YSPool_newNode(YSPool *pool,const char *s){
    //inserting string

    ssize_t sp = pool->bufLen;
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
    return YSTree_newNode(&pool->tree,sp);
}

size_t YSPool_addString(YSPool *pool,const char *s){
    //find node first.
    YSTree_prepareNode(&pool->tree);
    size_t *node = YSTree_find(&pool->tree,s);
    if(*node == 0){
        *node = YSPool_newNode(pool,s);
    }

    return YSTree_getNode(&pool->tree,*node)->sptr;
}

const char *YSPool_getString(YSPool *pool,size_t sp){
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
