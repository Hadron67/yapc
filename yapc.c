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

#include "yapc.h"
#include <stdlib.h>
#include <string.h>
#include "ydef.h"
#include "spool.h"
#include "parser.h"
#include "gen.h"
#include "ytest.h"
#include "code_gen.h"

struct _YContext{
    yheap_t heap[1];
    YGParser parser;
    YGrammar *g;

    YItemSetList doneList;
    YParseTable *table;
    YArray conflicts;

    const char *ysource;

    char *sbuf;
    int len,size;

    ybad_alloc_cb badAllocCb;
    void *badAllocArg;
};

static int YContext_pushc(YContext *ctx,char c){
    if(ctx->sbuf == NULL){
        ctx->size = 16;
        ctx->sbuf = (char *)ya_malloc(ctx->heap,sizeof(char) * ctx->size);
    }
    if(ctx->len >= ctx->size){
        ctx->size *= 2;
        ctx->sbuf = (char *)ya_realloc(ctx->heap,ctx->sbuf,sizeof(char) * ctx->size);
    }
    ctx->sbuf[ctx->len++] = c;
    return 0;
}

static void ybadAlloc(YContext *ctx){
    if(ctx->badAllocCb != NULL){
        ctx->badAllocCb(ctx->badAllocArg);
    }
    abort();
}

static void *yamalloc(size_t size,void *arg){
    void *ret = malloc(size);
    if(ret == NULL){
        ybadAlloc((YContext *)arg);
    }
    else{
        return ret;
    }
}

static void *yarealloc(void *ptr,size_t size,void *arg){
    void *ret = realloc(ptr,size);
    if(ret == NULL){
        ybadAlloc((YContext *)arg);
    }
    else{
        return ret;
    }
}

static void yafree(void *ptr,void *arg){
    free(ptr);
}

#define PUSHC(c) (YContext_pushc(ctx,(c)))

YContext *yNewContext(){
    yheap_t heap;
    heap.malloc = yamalloc;
    heap.realloc = yarealloc;
    heap.free = yafree;

    YContext *ret = (YContext *)ya_malloc(&heap,sizeof(YContext));
    ret->heap[0] = heap;
    ret->heap->arg = &ret;

    YArray_init(&ret->conflicts,sizeof(YConflict),8,ret->heap);
    YItemSetList_init(&ret->doneList);
    ret->table = NULL;
    ret->g = NULL;
    ret->ysource = NULL;
    ret->len = ret->size = 0;
    ret->sbuf = NULL;
    ret->badAllocCb = NULL;
    ret->badAllocArg = NULL;
    return ret;
}
void yDestroyContext(YContext *ctx1){
    YContext *ctx = (YContext *)ctx1;
    YArray_free(&ctx->conflicts,NULL);
    YItemSetList_clear(&ctx->doneList,ctx->heap);
    if(ctx->table != NULL){
        YParseTable_free(ctx->table);
    }
    if(ctx->g != NULL){
        YGrammar_free(ctx->g);
    }
    if(ctx->sbuf != NULL){
        ya_free(ctx->heap,ctx->sbuf);
    }
    ya_free(ctx->heap,ctx);
}

YAPC_API void ySetBadAllocCb(YContext *ctx,ybad_alloc_cb cb,void *arg){
    ctx->badAllocCb = cb;
    ctx->badAllocArg = arg;
}

int yParseFile(YContext *ctx,FILE *file,const char *fname,FILE *err){
    YGParser_init(&ctx->parser,err,NULL);
    ctx->g = YGParser_parse(&ctx->parser,file);
    ctx->ysource = fname;
    return ctx->g == NULL;
}
int yGenerateTable(YContext *ctx){
    if(ctx->g == NULL){
        return -1;
    }
    yGenItemSets(ctx->g,&ctx->doneList);
    ctx->table = yGenParseTable(ctx->g,&ctx->doneList,&ctx->conflicts);
}
int yPrintItemSets(YContext *ctx,int showLah,FILE *out){

    YItemSetList_dump(&ctx->doneList,showLah,out);
    return 0;
}
int yPrintTestResult(YContext *ctx,FILE *out){
    if(ctx->g == NULL){
        return -1;
    }
    yRunTests(ctx->g,ctx->table,out);
    return 0;
}
int yPrintGenerationWarnings(YContext *ctx,FILE *out){
    int count;
    YGrammar_printUnusedTokens(ctx->g,&count,out);
    if(count > 0){
        fprintf(out,"\n");
    }
    YGrammar_printUnusedNts(ctx->g,&count,out);
    if(count > 0){
        fprintf(out,"\n");
    }
    if(ctx->conflicts.len > 0){
        YConflicts_print(&ctx->conflicts,out);
        fprintf(out,"warning: %d conflict(s) detected.\n\n",ctx->conflicts.len);
    }
    return 0;
}
int yGenerateCCode(YContext *ctx,FILE *header,FILE *source,const char *headern,const char *sourcen){
    if(ctx->table == NULL || ctx->g == NULL){
        return -1;
    }
    yGenCCode(ctx->table,ctx->g,source,header,headern,sourcen,ctx->ysource);
    return 0;
}
int yConvertFileNames(YContext *ctx,const char *ysource,const char **header,const char **source,const char **out){
    if(ctx->sbuf != NULL){
        ya_free(ctx->heap,ctx->sbuf);
    }
    int len = strlen(ysource),len2;
    if(len >= 2 && ysource[len - 1] == 'y' && ysource[len - 2] == '.'){
        len2 = len - 2;
    }
    else {
        len2 = len;
    }
    ctx->sbuf = (char *)ya_malloc(ctx->heap,2 * sizeof(char) * (len2 + 3) + sizeof(char) * (len2 + 8));
    char *h = ctx->sbuf;
    char *s = h + len2 + 3;
    char *o = s + len2 + 3;
    int i;
    for(i = 0;i < len2;i++){
        h[i] = ysource[i];
        s[i] = ysource[i];
        o[i] = ysource[i];
    }
    strcpy(h + i,".h");
    strcpy(s + i,".c");
    strcpy(o + i,".output");

    *header = h;
    *source = s;
    *out = o;

    return 0;
}