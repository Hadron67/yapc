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

#define YTN(p) ((YTNode *)((char *)tree->buf + tree->nodeSize * (p)))

int YTree_init(YTree *tree,int size,size_t elemsize,ycomparator comp,void *arg){
    memset(tree,0,sizeof(YTree));
    
    //XXX:align needs to be token into account
    tree->elemSize = elemsize;
    tree->nodeSize = elemsize + sizeof(YTNode);
    tree->size = size;
    tree->len = 0;
    tree->root = YTNODE_NULL;
    tree->comp = comp;
    tree->arg = arg;

    tree->buf = (YTNode *)ya_malloc(tree->size * tree->nodeSize);

    return 0;
    
}
int YTree_reInit(YTree *tree){
    tree->len = 0;
    tree->root = YTNODE_NULL;
    return 0;
}
int YTree_free(YTree *tree){
    ya_free(tree->buf);
}
int YTree_getLength(YTree *tree){
    return tree->len;
}
static int YTree_prepareNode(YTree *tree){
    if(tree->len >= tree->size){
        tree->size *= 2;
        tree->buf = (YTNode *)ya_realloc(tree->buf,tree->nodeSize * tree->size);
    }
    return 0;
}
YTNode *YTree_getNode(YTree *tree,ynptr p){
    return YTN(p);
}
static ynptr YTree_newNode(YTree *tree,void **data){
    YTree_prepareNode(tree);
    ynptr p = tree->len++;
    YTNode *node = YTree_getNode(tree,p);
    node->left = node->right = node->parent = YTNODE_NULL;
    node->depth = 1;
    node->index = p;
    //memcpy(node->data,data,tree->elemSize);
    *data = node->data;
    return p;
}

ynptr *YTree_findEX(YTree *tree,const void *data,ynptr *node){
    YTree_prepareNode(tree);
    ynptr *tp = &tree->root;
    if(node != NULL){
        *node = tree->root;
    }
    while(*tp != YTNODE_NULL){
        int cmp = tree->comp(data,YTN(*tp)->data,tree->arg);
        if(node != NULL){
            *node = YTN(*tp)->index;
        }
        if(cmp > 0){
            tp = &YTN(*tp)->right;
        }
        else if(cmp < 0){
            tp = &YTN(*tp)->left;
        }
        else {
            break;
        }
    }
    return tp;
}
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define SET_LEFT(root,n) \
    assert((root) != (n)); \
    YTN(root)->left = (n); \
    if((n) != YTNODE_NULL) \
        YTN(n)->parent = (root);
#define SET_RIGHT(root,n) \
    assert((root) != (n)); \
    YTN(root)->right = (n); \
    if((n) != YTNODE_NULL) \
        YTN(n)->parent = (root);
#define DEPTH(node) ((node) == YTNODE_NULL ? 0 : YTN(node)->depth)
#define GET_DEPTH(n) (YTN(n)->depth = 1 + MAX(DEPTH(YTN(n)->left),DEPTH(YTN(n)->right)))

static void YTree_setInParent(YTree *tree,ynptr cela,ynptr nnode){
    if(cela == tree->root){
        //assert(cela->parent == NULL);
        tree->root = nnode;
        YTN(nnode)->parent = YTNODE_NULL;
    }
    else {
        if(cela == YTN(YTN(cela)->parent)->left){
            SET_LEFT(YTN(cela)->parent,nnode);
        }
        else {
            SET_RIGHT(YTN(cela)->parent,nnode);
        }
    }
}
/*
          A(root)             C
         / \                 / \
        B   C(top)          A   E
           / \   ====>     / \   \
          D   E           B   D   F
               \
                F
*/
static ynptr YTree_rotateRR(YTree *tree,ynptr root){
    ynptr top = YTN(root)->right;
    YTree_setInParent(tree,root,top);
    SET_RIGHT(root,YTN(top)->left);
    SET_LEFT(top,root);
    GET_DEPTH(root);
    GET_DEPTH(top);
    return top;
}
/*
        A(root)           B
       / \               / \
      B   C             D   A
     / \       ====>   /   / \
    D   E             F   E   C
   /
  F
*/
static ynptr YTree_rotateLL(YTree *tree,ynptr root){
    ynptr top = YTN(root)->left;
    YTree_setInParent(tree,root,top);
    SET_LEFT(root,YTN(top)->right);
    SET_RIGHT(top,root);
    GET_DEPTH(root);
    GET_DEPTH(top);
    return top;
}
/*
        A                A                E
       / \              / \              / \
      B   C            E   C            B   A
     / \      ====>   / \     ====>    /   / \
    D   E            B   F            D   F   C
         \          /
          F        D
*/
static ynptr YTree_rotateLR(YTree *tree,ynptr root){
    YTree_rotateRR(tree,YTN(root)->left);
    return YTree_rotateLL(tree,root);
}
/*
        A               A                D
       / \             / \              / \
      B   C           B   D            A   C
         / \   ====>     / \   ====>  / \   \
        D   E           F   C        B   F   E
       /                     \
      F                       E

*/
static ynptr YTree_rotateRL(YTree *tree,ynptr root){
    YTree_rotateLL(tree,YTN(root)->right);
    return YTree_rotateRR(tree,root);
}
static void YTree_checkBalance(YTree *tree,ynptr from){
    while(from != YTNODE_NULL){
        int dp;
        GET_DEPTH(from);
        dp = DEPTH(YTN(from)->left) - DEPTH(YTN(from)->right);
        if(dp >= 2){
            ynptr n = YTN(from)->left;
            if(DEPTH(YTN(n)->left) > DEPTH(YTN(n)->right)){
                from = YTree_rotateLL(tree,from);
            }
            else {
                from = YTree_rotateLR(tree,from);
            }
        }
        else if(dp <= -2){
            ynptr n = YTN(from)->right;
            if(DEPTH(YTN(n)->left) < DEPTH(YTN(n)->right)){
                from = YTree_rotateRR(tree,from);
            }
            else {
                from = YTree_rotateRL(tree,from);
            }
        }
        from = YTN(from)->parent;
    }  
}
void *YTree_insertAt(YTree *tree,ynptr node,ynptr *pos){
    void *ret;
    ynptr n = YTree_newNode(tree,&ret);
    YTNode *nnode = YTN(n),*onode = YTN(node);
    *pos = n;
    nnode->parent = node;
    YTree_checkBalance(tree,n);
    return ret;
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

static ysptr YSPool_addS(YSPool *pool,const char *s){
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

    return sp;
}

ysptr YSPool_addString(YSPool *pool,const char *s){
    //find node first.
    ynptr node;
    ynptr *pos = YTree_findEX(&pool->tree,s,&node);
    if(*pos == YTNODE_NULL){
        ysptr *ret = (ysptr *)YTree_insertAt(&pool->tree,node,pos);
        *ret = YSPool_addS(pool,s);
        return *ret;
    }
    else{
        return *(ysptr *)YTree_getNode(&pool->tree,*pos)->data;
    }
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
