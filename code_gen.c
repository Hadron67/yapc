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
#include <stdio.h>
#include <stdlib.h>
#include "code_gen.h"
#include "ydef.h"

typedef struct _YGen{
    const char *ysource,*source,*header;

    YParseTable *table;
    YGrammar *g;

    int line2;
}YGen;

#define YYSPACE 6

static int yPrintNum(FILE *out,int num){
    char buf[YYSPACE + 1];
    buf[YYSPACE] = '\0';
    int i = YYSPACE - 1;
    int neg = num < 0;
    if(neg){
        num = -num;
    }
    if(num == 0){
        buf[i--] = '0';
    }
    while(num > 0){
        buf[i--] = (num % 10) + '0';
        num /= 10;
    }
    if(neg){
        buf[i--] = '-';
    }
    while(i >= 0){
        buf[i--] = ' ';
    }
    fprintf(out,"%s",buf);
    return 0;
}

static int yGenShiftTable(YGen *g,FILE *out){
    fprintf(out,
        "/*\n"
        YYTAB "shift action table\n"
        YYTAB "positive numbers indicate the states shift to,\n"
        YYTAB "negative numbers indicate the rules reduce with.\n"
        YYTAB "the state should be the number in the table minus one,since zero marks\n"
        YYTAB "for error.\n"
        "*/\n"
    );
    g->line2 += 7;

    fprintf(out,"static const int %sshift[] = {\n",g->g->nameSpace);
    g->line2 += 1;
    int i,j,line = 0;
    for(i = 0;i < g->table->stateCount;i++){
        fprintf(out,YYTAB "/* state %d */\n" YYTAB,i);
        g->line2++;
        line = 0;
        for(j = 0;j < g->g->tokenCount;j++){
            YItem *item = g->table->shift[i * g->g->tokenCount + j];
            if(item == NULL){
                fprintf(out,"%6d,",0);
            }
            else if(item->actionType == YACTION_SHIFT){
                fprintf(out,"%6d,",item->shift->index + 1);
            }
            else if(item->actionType == YACTION_REDUCE){
                fprintf(out,"%6d,",-(item->rule->index + 1));
            }
            else {
                abort();
            }
            if(line++ >= 10){
                fprintf(out,"\n" YYTAB);
                line = 0;
                g->line2++;
            }
        }
        fprintf(out,"\n");
        g->line2++;
    }
    fprintf(out,"};\n");
    g->line2++;
}

static int yGenGotoTable(YGen *g,FILE *out){
    fprintf(out,
        "/*\n"
        YYTAB "goto table\n"
        "*/\n"
    );
    g->line2 += 3;

    fprintf(out,"static const int %sgoto[] = {\n",g->g->nameSpace);
    g->line2 += 1;
    int i,j,line = 0;
    for(i = 0;i < g->table->stateCount;i++){
        fprintf(out,YYTAB "/* state %d */\n" YYTAB,i);
        g->line2++;
        line = 0;
        for(j = 0;j < g->g->ntCount;j++){
            YItem *item = g->table->gotot[i * g->g->ntCount + j];
            if(item == NULL){
                fprintf(out,"%6d,",0);
            }
            else {
                assert(item->actionType == YACTION_SHIFT);
                fprintf(out,"%6d,",item->shift->index + 1);
            }
            if(line++ >= 10){
                fprintf(out,"\n" YYTAB);
                line = 0;
                g->line2++;
            }
        }
        fprintf(out,"\n");
        g->line2++;
    }
    fprintf(out,"};\n");
    g->line2++;
    return 0;
}

static int yGenLhsTable(YGen *g,FILE *out){
    const char *ns = g->g->nameSpace;
    fprintf(out,
        "/*\n"
        YYTAB "the left hand side of each rule,used to determine goto action.\n"
        "*/\n"
        "static const int %slhs[] = {\n"
        YYTAB 
        ,ns
    );
    g->line2 += 4;
    int i,line = 0;
    for(i = 0;i < g->g->ruleCount;i++){
        YRule *rule = g->g->rules + i;
        fprintf(out,"%6d,",rule->lhs);
        if(line++ >= 10){
            fprintf(out,"\n" YYTAB);
            line = 0;
            g->line2++;
        }
    }
    fprintf(out,"\n};\n");
    g->line2 += 2;
    return 0;
}

static int yyGenRuleLenTable(YGen *g,FILE *out){
    const char *const ns = g->g->nameSpace;
    fprintf(out,
        "/*\n"
        YYTAB "the length of the symbols on the rhs of each rule\n"
        YYTAB "used to pop states from the state stack when doing\n"
        YYTAB "an reduction.\n"
        "*/\n"
    );
    g->line2 += 5;

    fprintf(out,"static const int %sruleLen[] = {\n" YYTAB,ns);
    g->line2++;

    int i,line = 0;
    for(i = 0;i < g->g->ruleCount;i++){
        YRule *rule = g->g->rules + i;
        fprintf(out,"%6d,",rule->length);
        if(line++ >= 10){
            fprintf(out,"\n" YYTAB);
            line = 0;
            g->line2++;
        }
    }
    fprintf(out,"\n};\n");
    g->line2 += 2;
    return 0;
}

static int yPrintEscapedString(const char *s,FILE *out){
    while(*s){
        if(*s == '"'){
            fputc('\\',out);
        }
        fputc(*s,out);
        s++;
    }
    return 0;
}

static int yGenStringTable(YGen *g,FILE *out,const char *name,const char **s,int len,int align){
    fprintf(out,"const char *%s%s[] = {\n",g->g->nameSpace,name);
    g->line2++;
    int i,line = 0;
    fprintf(out,YYTAB);
    for(i = 0;i < len;i++){
        fprintf(out,"\"");
        yPrintEscapedString(s[i * align],out);
        fprintf(out,"\",");
        if(line++ >= 5){
            line = 0;
            fprintf(out,"\n" YYTAB);
            g->line2++;
        }
    }
    fprintf(out,"\n};\n");
    g->line2 += 2;
    return 0;
}
static int yGenSymbolTable(YGen *g,FILE *out){
    yGenStringTable(g,out,"tokenNames",&g->g->tokens->name,g->g->tokenCount,3);
    yGenStringTable(g,out,"tokenAlias",&g->g->tokens->alias,g->g->tokenCount,3);
    yGenStringTable(g,out,"nonTerminals",g->g->ntNames,g->g->ntCount,1);
    return 0;
}

static int yGenHeader(YGen *g,FILE *out){
    const char *ns = g->g->nameSpace;
    const char *stype = g->g->stype;
    fprintf(out,
        "/*\n"
        YYTAB "Generated by yapc -- An LR(1) parser generator made by CFY\n"
        "*/\n\n"
    );
    fprintf(out,
        "#ifndef __YY_H__\n"
        "#define __YY_H__\n\n"
        "#include <stdlib.h>\n"
        "#include <stdio.h>\n"
    );

    fprintf(out,
        "#define YY_OK 0\n"
        "#define YY_ERR -1\n"
    );


    int i;
    for(i = 0;i < g->g->tokenCount;i++){
        fprintf(out,"#define %s%s %d\n",g->g->tokenPrefix,g->g->tokens[i].alias,i);
    }

    fprintf(out,"\n\n");

    fprintf(out,
        "extern const char *%stokenNames[];\n"
        "extern const char *%stokenAlias[];\n"
        "extern const char *%snonTerminals[];\n"
        ,ns,ns,ns
    );

    fprintf(out,"\n\n");

    if(g->g->genCst){
        fprintf(out,
            "typedef struct _%sCst{\n"
            YYTAB "int isTermi;\n"
            YYTAB "int id;\n"
            YYTAB "int index;\n"
            YYTAB "int length;\n"
            YYTAB "int child,cousin;\n"
            YYTAB "//for printing\n"
            YYTAB "int printP;\n"
            YYTAB "int level;\n"
            "}%sCst;\n\n"
            ,ns
            ,ns
        );
    }

    fprintf(out,
        "typedef struct _%sParser{\n"
        YYTAB "//state stack\n"
        YYTAB "int *state;\n"
        YYTAB "int sLen,sSize;\n"
        
        YYTAB "//sematic stack\n"
        YYTAB "%s *pstack,*sp;\n"
        YYTAB "int pSize;\n"

        YYTAB "//current token,this token would be accepted\n"
        YYTAB "//when %sParser_acceptTokenis called.\n"
        YYTAB "%s token;\n"

        YYTAB "//this would be set to 1 when input is accepted.\n"
        YYTAB "int done;\n"

        YYTAB "//this would be set to 1 when a syntax error is detected.\n"
        YYTAB "int error,errToken;\n"

        YYTAB "//the generic pointer that user can set\n"
        YYTAB "void *userData;\n"
        ,ns
        ,stype
        ,ns
        ,stype
    );

    if(g->g->genCst){
        fprintf(out,
            YYTAB "//for construct concrete syntax tree.\n"
            YYTAB "%sCst *cst;\n"
            YYTAB "int cstLen,cstSize;\n"
            YYTAB "int *cStack;\n"
            YYTAB "int cLen,cSize;\n"
            ,ns
        );
    }

    fprintf(out,"}%sParser;\n",ns);

    fprintf(out,"\n\n");

    fprintf(out,
       "int %sParser_init(%sParser *%sparser);\n"
       "int %sParser_reInit(%sParser *%sparser);\n"
       "int %sParser_free(%sParser *%sparser);\n"
       "int %sParser_acceptToken(%sParser *%sparser,int %stokenid);\n"
       "int %sParser_printError(%sParser *%sparser,FILE *out);\n"
       "int %sParser_clearStack(%sParser *%sparser);\n"
       ,ns,ns,ns
       ,ns,ns,ns
       ,ns,ns,ns
       ,ns,ns,ns,ns
       ,ns,ns,ns
       ,ns,ns,ns
    );

    if(g->g->genCst){
        fprintf(out,
            "int %sParser_printCst(%sParser *%sparser,FILE *out);\n"
            ,ns,ns,ns
        );
    }

    fprintf(out,"\n\n");

    fprintf(out,"#endif");
}

static int yConvertActionBlock(YGen *g,FILE *out,const char *action,int stackOffset){
    fprintf(out,"{");
    int on = 1;
    while(*action){
        if(*action == '\n'){
            g->line2++;
        }
        if(on){
            if(*action == '$'){
                action++;
                if(*action == '$'){
                    action++;
                    fprintf(out,"%sval",g->g->nameSpace);
                }
                else if(*action >= '0' && *action <= '9'){
                    int num = *action - '0';
                    action++;
                    while(*action >= '0' && *action <= '9'){
                        num *= 10;
                        num += *action++ - '0';
                    }
                    fprintf(out,"(%sparser->sp[-%d])",g->g->nameSpace,stackOffset - num + 1);
                }
                else {
                    fputc('$',out);
                    fputc(*action,out);
                }
            }
            else if(*action == '"'){
                action++;
                fputc('"',out);
                on = !on;
            }
            else {
                fputc(*action,out);
                action++;
            }
        }
        else {  
            if(*action == '"'){
                action++;
                fputc('"',out);
                on = !on;
            }
            else if(*action == '\\'){
                action++;
                fputc('\\',out);
                if(*action){
                    fputc(*action,out);
                    action++;
                }
            }
            else {
                fputc(*action,out);
                action++;
            }
        }
    }
    fprintf(out,"}");
}

static int yGenReduce(YGen *g,FILE *out){
    const char *ns = g->g->nameSpace;
    const char *st = g->g->stype;
    fprintf(out,
        "static int %sParser_reduce(%sParser *%sparser,int %srule){\n"
        YYTAB "YYCHECK_PUSH_TOKEN();\n"
        YYTAB "%s %sval;\n"
        YYTAB "%s *%sdata = (%s *)%sparser->userData;\n"
        ,ns,ns,ns,ns
        ,st,ns
        ,g->g->dataType,ns,g->g->dataType,ns
    );
    g->line2 += 4;

    fprintf(out,
        YYTAB "switch(%srule){\n"
        ,ns
    );
    g->line2++;

    int i;
    for(i = 0;i < g->g->ruleCount;i++){
        YRule *rule = g->g->rules + i;
        fprintf(out,YYTAB YYTAB "case %d:\n",i);
        g->line2++;
        fprintf(out,YYTAB YYTAB YYTAB "/* ");
        YRule_dump(g->g,rule,out);
        fprintf(out," */\n");
        g->line2++;
        if(rule->actionBlock){
            fprintf(out,YYTAB YYTAB YYTAB "#line %d \"%s\"\n",rule->line,g->ysource);
            g->line2++;
            fprintf(out,YYTAB YYTAB YYTAB);
            yConvertActionBlock(g,out,rule->actionBlock,rule->stackOffset);
            fprintf(out,"\n");
            g->line2++;
            fprintf(out,YYTAB YYTAB YYTAB "#line %d \"%s\"\n",g->line2 + 1,g->source);
            g->line2++;
        }
        else {
            fprintf(out,YYTAB YYTAB YYTAB "/* no action. */\n");
            fprintf(out,
                YYTAB YYTAB YYTAB "%sval = %sparser->sp[%d];\n"
                ,g->g->nameSpace,g->g->nameSpace
                ,-rule->stackOffset
            );
            g->line2 += 2;
        }
        if(!rule->isGen){
            if(rule->stackOffset > 0){
                fprintf(out,
                    YYTAB YYTAB YYTAB "%sparser->sp -= %d;\n"
                    ,g->g->nameSpace,rule->stackOffset
                );
                g->line2++;
            }
            fprintf(out,
                YYTAB YYTAB YYTAB "*%sparser->sp++ = %sval;\n"
                ,g->g->nameSpace,g->g->nameSpace
            );
            g->line2++;
        }
        else {
            //if a rule is generated,its length is always zero,
            //so neednt pop the sematic stack.
            assert(rule->length == 0);
            if(rule->hasValue){
                fprintf(out,
                    YYTAB YYTAB YYTAB "*%sparser->sp++ = %sval;\n"
                    ,g->g->nameSpace,g->g->nameSpace
                );
                g->line2++;
            }
        }
        fprintf(out,YYTAB YYTAB YYTAB "break;\n");
        g->line2++;
    }

    fprintf(out,YYTAB "}\n");
    g->line2++;

    fprintf(out,
        YYTAB "%sparser->sLen -= %sruleLen[%srule];\n"
        ,ns,ns,ns
    );
    g->line2 += 1;

    fprintf(out,
        YYTAB "int %sindex = YYSTATE() * %sntCount + %slhs[%srule];\n"
        YYTAB "YYPUSH_STATE(%sgoto[%sindex] - 1);\n"
        ,ns,ns,ns,ns
        ,ns,ns
    );
    g->line2 += 2;

    if(g->g->genCst){
        fprintf(out,
            YYTAB "%sParser_pushCst(%sparser,0,%slhs[%srule],%sruleLen[%srule]);\n"
            ,ns,ns,ns,ns,ns,ns
        );
    }

    fprintf(out,YYTAB "return 0;\n");
    fprintf(out,"}\n");
    g->line2 += 2;
}

static int yGenParseCode(YGen *g,FILE *out){
    const char *ns = g->g->nameSpace;
    const char *stype = g->g->stype;
    fprintf(out,
        "/*\n"
        YYTAB "Generated by yapc -- An LR(1) parser generator made by CFY\n"
        "*/\n\n"
    );
    g->line2 += 4;
    //prologue
    const char *s = g->g->prologue;
    while(*s){
        if(*s == '\n'){
            g->line2++;
        }
        fputc(*s,out);
        s++;
    }

    fprintf(out,
        "#include \"%s\"\n"
        ,g->header
    );
    g->line2++;

    fprintf(out,
        "static const int %stokenCount = %d;\n"
        "static const int %sntCount = %d;\n"
        ,ns,g->g->tokenCount
        ,ns,g->g->ntCount
    );
    g->line2 += 2;

    fprintf(out,
        "#ifndef YYMALLOC\n"
        YYTAB "#define YYMALLOC malloc\n"
        "#endif\n"
        "#ifndef YYREALLOC\n"
        YYTAB "#define YYREALLOC realloc\n"
        "#endif\n"
        "#ifndef YYFREE\n"
        YYTAB "#define YYFREE free\n"
        "#endif\n"
    );
    g->line2 += 9;

    fprintf(out,
        "#ifndef YYDESTRUCTOR\n"
        YYTAB "#define YYDESTRUCTOR(a)\n"
        "#endif\n"
    );
    g->line2 += 3;

    fprintf(out,
        "#define YYPUSH_STATE(s) \\\n"
        YYTAB "if(%sparser->sLen >= %sparser->sSize){ \\\n"
        YYTAB YYTAB "%sparser->sSize *= 2; \\\n"
        YYTAB YYTAB "%sparser->state = (int *)YYREALLOC(%sparser->state,sizeof(int) * %sparser->sSize); \\\n"
        YYTAB "} \\\n"
        YYTAB "%sparser->state[%sparser->sLen++] = (s);\n"
        "\n"
        "#define YYSTATE() (%sparser->state[%sparser->sLen - 1])\n"
        ,ns,ns
        ,ns
        ,ns,ns,ns
        ,ns,ns
        ,ns,ns
    );
    g->line2 += 8;

    fprintf(out,
        "#define YYCHECK_PUSH_TOKEN() \\\n"
        YYTAB "if(%sparser->sp - %sparser->pstack >= %sparser->pSize){\\\n"
        YYTAB YYTAB "size_t offset = %sparser->sp - %sparser->pstack;\\\n"
        YYTAB YYTAB "%sparser->pSize *= 2;\\\n"
        YYTAB YYTAB "%sparser->pstack = (%s *)YYREALLOC(%sparser->pstack,sizeof(%s) * %sparser->pSize);\\\n"
        YYTAB YYTAB "%sparser->sp = %sparser->pstack + offset;\\\n"
        YYTAB "}\n"
        ,ns,ns,ns
        ,ns,ns
        ,ns
        ,ns,stype,ns,stype,ns
        ,ns,ns
    );
    g->line2 += 7;

    yGenShiftTable(g,out);
    yGenGotoTable(g,out);
    yGenLhsTable(g,out);
    yyGenRuleLenTable(g,out);
    //yGenRuleLengthTable(g,out);
    //yGenRuleValueLengthTable(g,out);
    yGenSymbolTable(g,out);
    if(g->g->genCst){
        fprintf(out,
            "static int %sParser_pushCst(%sParser *parser,int isT,int id,int length){\n"
            YYTAB "if(parser->cstLen >= parser->cstSize){\n"
            YYTAB YYTAB "parser->cstSize *= 2;\n"
            YYTAB YYTAB "parser->cst = (%sCst *)YYREALLOC(parser->cst,sizeof(%sCst) * parser->cstSize);\n"
            YYTAB "}\n"
            YYTAB "if(parser->cLen >= parser->cSize){\n"
            YYTAB YYTAB "parser->cSize *= 2;\n"
            YYTAB YYTAB "parser->cStack = (int *)YYREALLOC(parser->cStack,sizeof(int) * parser->cSize);\n"
            YYTAB "}\n"
            YYTAB "%sCst *cst = parser->cst + parser->cstLen++;\n"
            YYTAB "cst->index = parser->cstLen - 1;\n"
            YYTAB "cst->id = id;\n"
            YYTAB "cst->isTermi = isT;\n"
            YYTAB "cst->level = 0;\n"
            YYTAB "cst->length = length;\n"
            YYTAB "cst->child = cst->cousin = -1;\n"
            YYTAB "if(!isT){\n"
            YYTAB YYTAB "int i;\n"
            YYTAB YYTAB "for(i = parser->cLen - length;i < parser->cLen - 1;i++){\n"
            YYTAB YYTAB YYTAB "parser->cst[parser->cStack[i]].cousin = parser->cst[parser->cStack[i + 1]].index;\n"
            YYTAB YYTAB "}\n"
            YYTAB YYTAB "cst->child = parser->cst[parser->cStack[parser->cLen - length]].index;\n"
            YYTAB YYTAB "parser->cLen -= length;\n"
            YYTAB "}\n"
            YYTAB "parser->cStack[parser->cLen++] = cst->index;\n"
            YYTAB "return 0;\n"
            "}\n"
            ,ns,ns

            ,ns,ns
            ,ns
        );
        g->line2 += 27;
    }
    yGenReduce(g,out);


    fprintf(out,
        "int %sParser_init(%sParser *%sparser){\n"
        YYTAB "%sparser->sLen = 1;\n"
        YYTAB "%sparser->done = 0;\n"
        YYTAB "%sparser->sSize = %sparser->pSize = 16;\n"
        YYTAB "%sparser->state = (int *)YYMALLOC(sizeof(int) * %sparser->sSize);\n"
        YYTAB "%sparser->state[0] = 0;\n"
        YYTAB "%sparser->sp = %sparser->pstack = (%s *)YYMALLOC(sizeof(%s) * %sparser->pSize);\n"
        ,ns,ns,ns
        ,ns
        ,ns
        ,ns,ns
        ,ns,ns
        ,ns
        ,ns,ns,stype,stype,ns
    );
    g->line2 += 7;

    if(g->g->genCst){
        fprintf(out,
            YYTAB "%sparser->cstSize = 64;\n"
            YYTAB "%sparser->cstLen = 0;\n"
            YYTAB "%sparser->cst = (%sCst *)YYMALLOC(sizeof(%sCst) * %sparser->cstSize);\n"
            YYTAB "%sparser->cLen = 0;\n"
            YYTAB "%sparser->cSize = 16;\n"
            YYTAB "%sparser->cStack = (int *)YYMALLOC(sizeof(int) * %sparser->cSize);\n"
            ,ns
            ,ns
            ,ns,ns,ns,ns

            ,ns
            ,ns
            ,ns,ns
        );
        g->line2 += 6;
    }

    fprintf(out,
        YYTAB "return 0;\n"
        "}\n"
    );
    g->line2 += 2;

    fprintf(out,
        "int %sParser_reInit(%sParser *%sparser){\n"
        YYTAB "%sparser->sLen = 0;\n"
        YYTAB "%sparser->done = 0;\n"
        YYTAB "%sparser->state[0] = 0;\n"
        YYTAB "%sparser->sp = %sparser->pstack;\n"
        ,ns,ns,ns
        ,ns
        ,ns
        ,ns
        ,ns,ns
    );
    g->line2 += 5;

    if(g->g->genCst){
        fprintf(out,
            YYTAB "%sparser->cstLen = 0;\n"
            YYTAB "%sparser->cLen = 0;\n"
            ,ns
            ,ns
        );
        g->line2 += 2;
    }

    fprintf(out,
        YYTAB "return 0;\n"
        "}\n"
    );
    g->line2 += 2;

    fprintf(out,
        "int %sParser_free(%sParser *%sparser){\n"
        YYTAB "YYFREE(%sparser->state);\n"
        YYTAB "YYFREE(%sparser->pstack);\n"
        ,ns,ns,ns
        ,ns
        ,ns
    );
    g->line2 += 3;

    if(g->g->genCst){
        fprintf(out,
            "YYFREE(%sparser->cst);\n"
            "YYFREE(%sparser->cStack);\n"
            ,ns
            ,ns
        );
        g->line2 += 2;
    }

    fprintf(out,
        YYTAB "return 0;\n"
        "}\n"
    );
    g->line2 += 2;

    fprintf(out,
        "int %sParser_acceptToken(%sParser *%sparser,int %stokenid){\n"
        YYTAB "int shifted = 0;\n"
        YYTAB "while(!shifted){\n"
        YYTAB YYTAB "int %saction = %sshift[YYSTATE() * %stokenCount + %stokenid];\n"
        YYTAB YYTAB "if(%saction > 0){\n"
        YYTAB YYTAB YYTAB "YYCHECK_PUSH_TOKEN();\n"
        YYTAB YYTAB YYTAB "*%sparser->sp++ = %sparser->token;\n"
        YYTAB YYTAB YYTAB "YYPUSH_STATE(%saction - 1);\n"
        YYTAB YYTAB YYTAB "shifted = 1;\n"
        ,ns,ns,ns,ns
        ,ns,ns,ns,ns
        ,ns
        ,ns,ns
        ,ns
    );
    g->line2 += 9;

    if(g->g->genCst){
        fprintf(out,
            YYTAB YYTAB YYTAB "%sParser_pushCst(%sparser,1,%stokenid,0);\n"
            ,ns,ns,ns
        );
    }

    fprintf(out,
        YYTAB YYTAB "}\n"
        YYTAB YYTAB "else if(%saction < 0){\n"
        YYTAB YYTAB YYTAB "if(%saction == -1){\n"
        YYTAB YYTAB YYTAB YYTAB "%sparser->done = 1;\n"
        YYTAB YYTAB YYTAB YYTAB "return YY_OK;\n"
        YYTAB YYTAB YYTAB "}\n"
        YYTAB YYTAB YYTAB "%sParser_reduce(%sparser,-1 - %saction);\n"
        YYTAB YYTAB "}\n"
        YYTAB YYTAB "else {\n"
        YYTAB YYTAB YYTAB "%sparser->error = 1;\n"
        YYTAB YYTAB YYTAB "%sparser->errToken = %stokenid;\n"
        YYTAB YYTAB YYTAB "return YY_ERR;\n"
        YYTAB YYTAB "}\n"
        YYTAB "}\n"
        YYTAB "return YY_OK;\n"
        "}\n"
        ,ns
        ,ns
        ,ns
        ,ns,ns,ns
        ,ns
        ,ns,ns
    );
    g->line2 += 16;

    fprintf(out,
        "int %sParser_printError(%sParser *%sparser,FILE *out){\n"
        YYTAB "if(%sparser->error){\n"
        YYTAB YYTAB "int index = YYSTATE() * %stokenCount;\n"
        YYTAB YYTAB "fprintf(out,\"unexpected token '%cs' (%cs),was expecting one of:\\n\",%stokenNames[%sparser->errToken],%stokenAlias[%sparser->errToken]);\n"
        YYTAB YYTAB "int i;\n"
        YYTAB YYTAB "for(i = 0;i < %stokenCount;i++){\n"
        YYTAB YYTAB YYTAB "if(%sshift[index + i] != 0){\n"
        YYTAB YYTAB YYTAB YYTAB "fprintf(out,\"    '%cs' (%cs) ...\\n\",%stokenNames[i],%stokenAlias[i]);\n"
        YYTAB YYTAB YYTAB "}\n"
        YYTAB YYTAB "}\n"
        YYTAB "}\n"
        YYTAB "return YY_OK;\n"
        "}\n"
        ,ns,ns,ns
        ,ns
        ,ns
        ,'%','%',ns,ns,ns,ns
        ,ns
        ,ns
        ,'%','%',ns,ns
    );
    g->line2 += 13;

    fprintf(out,
        "int %sParser_clearStack(%sParser *%sparser){\n"
        YYTAB "while(%sparser->sp > %sparser->pstack){\n"
        YYTAB YYTAB "%sparser->sp--;\n"
        YYTAB YYTAB "YYDESTRUCTOR(%sparser->sp);\n"
        YYTAB "}\n"
        YYTAB "return 0;\n"
        "}\n"
        ,ns,ns,ns
        ,ns,ns
        ,ns
        ,ns
    );
    g->line2 += 7;

    if(g->g->genCst){
        fprintf(out,
            "/*\n"
            YYTAB "following functions is for printing syntax trees in an elegant way.\n"
            "*/\n"
        );
        g->line2 += 3;

        fprintf(out,
            "#define YYCCHR_DOWN \"│   \"\n"
            "#define YYCCHR_CONNOR \"└───\"\n"
            "#define YYCCHR_DIVERSE \"├───\"\n"
            "#define YYCCHR_BLANK \"    \"\n"
        );
        g->line2 += 4;
        
        fprintf(out,
            "static void repeatPrint(FILE *out,const char *s,int count){\n"
            YYTAB "while(count --> 0) fprintf(out,\"%%s\",s);\n"
            "}\n"
        );
        g->line2 += 3;

        fprintf(out,
            "static %sCst *%sCst_getChild(%sCst *array,%sCst *cst,int index){\n"
            YYTAB "cst = array + cst->child;\n"
            YYTAB "while(index --> 0){\n"
            YYTAB YYTAB "cst = array + cst->cousin;\n"
            YYTAB "}\n"
            YYTAB "return cst;\n"
            "}\n"
            ,ns,ns,ns,ns

        );
        g->line2 += 7;

        fprintf(out,
            "static int %sCst_printNode(%sCst *cst,%sCst **stack,int sp,FILE *out){\n"
            YYTAB "int i,level = 0;\n"
            YYTAB "for(i = 0;i < sp - 1;i++){\n"
            YYTAB YYTAB "repeatPrint(out,YYCCHR_BLANK,stack[i]->level - level - 1);\n"
            YYTAB YYTAB "fprintf(out,YYCCHR_DOWN);\n"
            YYTAB YYTAB "level = stack[i]->level;\n"
            YYTAB "}\n"
            YYTAB "if(sp > 0){\n"
            YYTAB YYTAB "repeatPrint(out,YYCCHR_BLANK,stack[i]->level - level - 1);\n"
            YYTAB YYTAB "if(stack[i]->printP < stack[i]->length - 1){\n"
            YYTAB YYTAB YYTAB "fprintf(out,YYCCHR_DIVERSE);\n"
            YYTAB YYTAB "}\n"
            YYTAB YYTAB "else {\n"
            YYTAB YYTAB YYTAB "fprintf(out,YYCCHR_CONNOR);\n"
            YYTAB YYTAB "}\n"
            YYTAB "}\n"
            YYTAB "if(cst->isTermi){\n"
            YYTAB YYTAB "fprintf(out,\"<%%s>\\n\",%stokenNames[cst->id]);\n"
            YYTAB "}\n"
            YYTAB "else{\n"
            YYTAB YYTAB "fprintf(out,\"%%s\\n\",%snonTerminals[cst->id]);\n"
            YYTAB "}\n"
            YYTAB "return YY_OK;\n"
            "}\n"
            ,ns,ns,ns

            ,ns
            ,ns
        );
        g->line2 += 23;

        fprintf(out,
            "int %sParser_printCst(%sParser *parser,FILE *out){\n"
            YYTAB "%sCst **cstack = (%sCst **)YYMALLOC(sizeof(%sCst *) * parser->cstLen);\n"
            YYTAB "int sp = 0;\n"
            YYTAB "%sCst *root = &parser->cst[parser->cStack[0]];\n"
            YYTAB "root->printP;\n"
            YYTAB "root->level = 1;\n"
            YYTAB "%sCst_printNode(root,cstack,sp,out);\n"
            YYTAB "cstack[sp++] = root;\n"
            YYTAB "while(sp > 0){\n"
            YYTAB YYTAB "%sCst *node = cstack[sp - 1];\n"
            YYTAB YYTAB "%sCst *childn = %sCst_getChild(parser->cst,node,node->printP);\n"
            YYTAB YYTAB "childn->level = node->level + 1;\n"
            YYTAB YYTAB "%sCst_printNode(childn,cstack,sp,out);\n"
            YYTAB YYTAB "if(++node->printP >= node->length){\n"
            YYTAB YYTAB YYTAB "sp--;\n"
            YYTAB YYTAB "}\n"
            YYTAB YYTAB "if(childn->length > 0){\n"
            YYTAB YYTAB YYTAB "cstack[sp++] = childn;\n"
            YYTAB YYTAB YYTAB "childn->printP = 0;\n"
            YYTAB YYTAB "}\n"
            YYTAB "}\n"
            YYTAB "YYFREE(cstack);\n"
            YYTAB "return YY_OK;\n"
            "}\n"
            ,ns,ns
            ,ns,ns,ns
            
            ,ns
            ,ns

            ,ns
            ,ns,ns
            ,ns
        );
        g->line2 += 24;
    }

    return 0;
}

int yGenCCode(YParseTable *table,YGrammar *g,FILE *cFile,FILE *header,const char *headern,const char *source,const char *ysource){
    YGen ge;
    ge.table = table;
    ge.g = g;
    ge.line2 = 1;

    ge.source = source;
    ge.header = headern;
    ge.ysource = ysource;

    yGenHeader(&ge,header);

    yGenParseCode(&ge,cFile);

}