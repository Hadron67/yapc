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

#ifndef __YAPC_H__
#define __YAPC_H__

#include "ydef.h"
#include <stdio.h>

typedef struct _YContext YContext;

typedef void (*ybad_alloc_cb)(void *arg);

YAPC_API YContext *yNewContext();
YAPC_API void yDestroyContext(YContext *ctx);

YAPC_API void ySetBadAllocCb(YContext *ctx,ybad_alloc_cb cb,void *arg);

YAPC_API int yParseFile(YContext *ctx,FILE *file,const char *fname,FILE *err);
YAPC_API int yGenerateTable(YContext *ctx);
YAPC_API int yPrintItemSets(YContext *ctx,int showLah,FILE *out);
YAPC_API int yPrintGenerationWarnings(YContext *ctx,FILE *out);
YAPC_API int yPrintTestResult(YContext *ctx,FILE *out);
YAPC_API int yGenerateCCode(YContext *ctx,FILE *header,FILE *source,const char *headern,const char *sourcen);

YAPC_API int yConvertFileNames(YContext *ctx,const char *ysource,const char **header,const char **source,const char **out);

#endif