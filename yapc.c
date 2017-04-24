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

typedef struct _YContext{
    YGParser parser;
    YGrammar *g;

    YItemSetList doneList;
    YParseTable *table;
    YConflictList cList;

    const char *ysource;

    char *sbuf;
    int len,size;
}YContext;

static int YContext_pushc(YContext *ctx,char c){
    if(ctx->sbuf == NULL){
        ctx->size = 16;
        ctx->sbuf = (char *)ya_malloc(sizeof(char) * ctx->size);
    }
    if(ctx->len >= ctx->size){
        ctx->size *= 2;
        ctx->sbuf = (char *)ya_realloc(ctx->sbuf,sizeof(char) * ctx->size);
    }
    ctx->sbuf[ctx->len++] = c;
    return 0;
}

#define PUSHC(c) (YContext_pushc(ctx,(c)))

void *yNewContext(){
    YContext *ret = (YContext *)ya_malloc(sizeof(YContext));
    YConflictList_init(&ret->cList,8);
    YItemSetList_init(&ret->doneList);
    ret->table = NULL;
    ret->g = NULL;
    ret->ysource = NULL;
    ret->len = ret->size = 0;
    ret->sbuf = NULL;
    return ret;
}
void yDestroyContext(void *ctx1){
    YContext *ctx = (YContext *)ctx1;
    YConflictList_free(&ctx->cList);
    YItemSetList_clear(&ctx->doneList);
    if(ctx->g != NULL){
        YGrammar_free(ctx->g);
    }
    if(ctx->table != NULL){
        YParseTable_free(ctx->table);
    }
    if(ctx->sbuf != NULL){
        ya_free(ctx->sbuf);
    }
    ya_free(ctx);
}

int yParseFile(void *ctx1,FILE *file,const char *fname,FILE *err){
    YContext *ctx = (YContext *)ctx1;
    YGParser_init(&ctx->parser,err);
    ctx->g = YGParser_parse(&ctx->parser,file);
    ctx->ysource = fname;
    return ctx->g == NULL;
}
int yGenerateTable(void *ctx1){
    YContext *ctx = (YContext *)ctx1;
    if(ctx->g == NULL){
        return -1;
    }
    yGenItemSets(ctx->g,&ctx->doneList);
    ctx->table = yGenParseTable(ctx->g,&ctx->doneList,&ctx->cList);
}
int yPrintResult(void *ctx1,FILE *out){
    YContext *ctx = (YContext *)ctx1;

    YItemSetList_dump(&ctx->doneList,1,out);
    return 0;
}
int yPrintTestResult(void *ctx1,FILE *out){
    YContext *ctx = (YContext *)ctx1;
    if(ctx->g == NULL){
        return -1;
    }
    yRunTests(ctx->g,ctx->table,out);
    return 0;
}
int yPrintGenerationWarnings(void *ctx1,FILE *out){
    YContext *ctx = (YContext *)ctx1;
    YConflictList_print(&ctx->cList,out);
    return 0;
}
int yGenerateCCode(void *ctx1,FILE *header,FILE *source,const char *headern,const char *sourcen){
    YContext *ctx = (YContext *)ctx1;
    if(ctx->table == NULL || ctx->g == NULL){
        return -1;
    }
    yGenCCode(ctx->table,ctx->g,source,header,headern,sourcen,ctx->ysource);
    return 0;
}
int yConvertFileNames(void *ctx1,const char *ysource,const char **header,const char **source,const char **out){
    YContext *ctx = (YContext *)ctx1;
    if(ctx->sbuf != NULL){
        ya_free(ctx->sbuf);
    }
    int len = strlen(ysource),len2;
    if(len >= 2 || ysource[len - 1] == 'y' || ysource[len - 2] == '.'){
        len2 = len - 2;
    }
    else {
        len2 = len;
    }
    ctx->sbuf = (char *)ya_malloc(2 * sizeof(char) * (len2 + 3) + sizeof(char) * (len2 + 8));
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