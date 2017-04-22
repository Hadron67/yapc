#ifndef __CODE_GEN_H__
#define __CODE_GEN_H__

#include "grammar.h"
#include "gen.h"

int yGenCCode(YParseTable *table,YGrammar *g,FILE *cFile,FILE *header,const char *headern,const char *source,const char *ysource);


#endif