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

#include <stdio.h>

void *yNewContext();
void yDestroyContext(void *ctx);

int yParseFile(void *ctx,FILE *file,const char *fname,FILE *err);
int yGenerateTable(void *ctx);
int yPrintResult(void *ctx,FILE *out);
int yPrintGenerationWarnings(void *ctx,FILE *out);
int yPrintTestResult(void *ctx,FILE *out);
int yGenerateCCode(void *ctx,FILE *header,FILE *source,const char *headern,const char *sourcen);

int yConvertFileNames(void *ctx,const char *ysource,const char **header,const char **source,const char **out);

#endif