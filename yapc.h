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