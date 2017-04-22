#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "code_gen.h"

typedef struct _YGen{
    const char *ysource,*source,*header;

    YParseTable *table;
    YGrammar *g;

    int line2;
}YGen;

#define YYSPACE 6
#define YYTAB "    "

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
        "/** shift action table\n"
        " * positive numbers indicate the states shift to,\n"
        " * negative numbers indicate the rules reduce with.\n"
        " * the state should be the number in the table minus one,since zero marks\n"
        " * for error.*/\n"
    );
    g->line2 += 5;
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
                yPrintNum(out,0);
                fprintf(out,",");
            }
            else if(item->actionType == YACTION_SHIFT){
                yPrintNum(out,item->shift->index + 1);
                fprintf(out,",");
            }
            else if(item->actionType == YACTION_REDUCE){
                yPrintNum(out,-(item->rule->index + 1));
                fprintf(out,",");
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
        "/** goto table,\n"
        " * zero iff there is an error*/\n"
    );
    fprintf(out,"static const int %sgoto[] = {\n",g->g->nameSpace);
    g->line2 += 3;
    int i,j,line = 0;
    for(i = 0;i < g->table->stateCount;i++){
        fprintf(out,YYTAB "/* state %d */\n" YYTAB,i);
        g->line2++;
        line = 0;
        for(j = 0;j < g->g->ntCount;j++){
            YItem *item = g->table->gotot[i * g->g->ntCount + j];
            if(item == NULL){
                yPrintNum(out,0);
                fprintf(out,",");
            }
            else {
                assert(item->actionType == YACTION_SHIFT);
                yPrintNum(out,item->shift->index + 1);
                fprintf(out,",");
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
        "/* the lhs of each rule. */\n"
        "static const int %slhs[] = {\n"
        YYTAB 
        ,ns
    );
    g->line2 += 2;
    int i,line = 0;
    for(i = 0;i < g->g->ruleCount;i++){
        YRule *rule = g->g->rules + i;
        yPrintNum(out,rule->lhs);
        fprintf(out,",");
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

static int yGenStringTable(YGen *g,FILE *out,const char *name,const char **s,int len){
    fprintf(out,"static const char *%s%s[] = {\n",g->g->nameSpace,name);
    g->line2++;
    int i,line = 0;
    fprintf(out,YYTAB);
    for(i = 0;i < len;i++){
        fprintf(out,"\"");
        yPrintEscapedString(s[i],out);
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
    yGenStringTable(g,out,"tokenNames",g->g->tokenNames,g->g->tokenCount);
    yGenStringTable(g,out,"tokenAlias",g->g->tokenAlias,g->g->tokenCount);
    yGenStringTable(g,out,"nonTerminals",g->g->ntNames,g->g->ntCount);
    return 0;
}

static int yGenHeader(YGen *g,FILE *out){
    const char *ns = g->g->nameSpace;
    const char *stype = g->g->stype;
    fprintf(out,
        "#ifndef __YY_H__\n"
        "#define __YY_H__\n\n"
        "#include <stdlib.h>\n"
        "#include <stdio.h>\n"
    );
    int i;
    for(i = 0;i < g->g->tokenCount;i++){
        fprintf(out,"#define %s%s %d\n",g->g->tokenPrefix,g->g->tokenAlias[i],i);
    }

    fprintf(out,"\n\n");

    fprintf(out,
        "typedef void *(*%salloc)(size_t size);\n"
        "typedef void *(*%srealloc)(void *p,size_t size);\n"
        "typedef void (*%sfree)(void *p);\n"
        ,ns,ns,ns
    );

    fprintf(out,"\n\n");

    fprintf(out,
        "typedef struct _%sParser{\n"
        YYTAB "%salloc altor;\n"
        YYTAB "%sfree dtor;\n"
        YYTAB "%srealloc rtor;\n"

        YYTAB "//state stack\n"
        YYTAB "int *state;\n"
        YYTAB "int sLen,sSize;\n"
        
        YYTAB "//sematic stack\n"
        YYTAB "%s *pstack,*sp;\n"
        YYTAB "int pSize;\n"

        YYTAB "//current token,this token would be accepted\n"
        YYTAB "//when %sParser_acceptTokenis called.\n"
        YYTAB "%s token;\n"

        YYTAB "//this would be set to 1 when a syntax error is detected.\n"
        YYTAB "int error,errToken;\n"
        "}%sParser;\n"
        ,ns,ns,ns,ns
        ,stype
        ,ns
        ,stype
        ,ns
    );

    fprintf(out,"\n\n");

    fprintf(out,
       "int %sParser_init(%sParser *%sparser,%salloc altor,%srealloc rtor,%sfree dtor);\n"
       "int %sParser_free(%sParser *%sparser);\n"
       "int %sParser_acceptToken(%sParser *%sparser,int %stokenid);\n"
       "int %sParser_printError(%sParser *%sparser,FILE *out);\n"
       ,ns,ns,ns,ns,ns,ns
       ,ns,ns,ns
       ,ns,ns,ns,ns
       ,ns,ns,ns
    );

    fprintf(out,"\n\n");

    fprintf(out,"#endif");
}

static int yConvertActionBlock(YGen *g,FILE *out,const char *action,int stackOffset){
    fprintf(out,"{ ");
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
    fprintf(out,
        "static int %sParser_reduce(%sParser *%sparser,int %srule){\n"
        YYTAB "YYCHECK_PUSH_TOKEN();\n"
        YYTAB "%s %sval;\n"
        YYTAB "switch(%srule){\n"
        ,g->g->nameSpace,g->g->nameSpace,g->g->nameSpace,g->g->nameSpace
        ,g->g->stype,g->g->nameSpace
        ,g->g->nameSpace
    );
    g->line2 += 4;

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
            g->line2++;
        }
        if(!rule->isGen){
            if(rule->stackOffset > 0){
                fprintf(out,
                    YYTAB YYTAB YYTAB "%sparser->sp -= %d;\n"
                    ,g->g->nameSpace,rule->stackOffset
                );
                g->line2++;
            }
            if(rule->length > 0){
                fprintf(out,
                    YYTAB YYTAB YYTAB "%sparser->sLen -= %d;\n"
                    ,g->g->nameSpace,rule->length
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
        YYTAB "int %sindex = YYSTATE() * %sntCount + %slhs[%srule];\n"
        YYTAB "YYPUSH_STATE(%sparser,%sgoto[%sindex] - 1);\n"
        ,g->g->nameSpace,g->g->nameSpace,g->g->nameSpace,g->g->nameSpace
        ,g->g->nameSpace,g->g->nameSpace,g->g->nameSpace
    );
    g->line2++;

    fprintf(out,YYTAB "return 0;\n");
    fprintf(out,"}\n");
    g->line2 += 2;
}

static int yGenParseCode(YGen *g,FILE *out){
    const char *ns = g->g->nameSpace;
    const char *stype = g->g->stype;

    fprintf(out,
        "#include \"%s\"\n"
        ,g->header
    );
    g->line2++;

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
        "static int %stokenCount = %d;\n"
        "static int %sntCount = %d;\n"
        ,ns,g->g->tokenCount
        ,ns,g->g->ntCount
    );
    g->line2 += 2;

    fprintf(out,
        "#define YYPUSH_STATE(parser,s) \\\n"
        YYTAB "if(parser->sLen >= parser->sSize){ \\\n"
        YYTAB YYTAB "parser->sSize *= 2; \\\n"
        YYTAB YYTAB "parser->state = (int *)parser->rtor(parser->state,sizeof(int) * parser->sSize); \\\n"
        YYTAB "} \\\n"
        YYTAB "parser->state[parser->sLen++] = (s);\n"
        "\n"
        "#define YYSTATE() (%sparser->state[%sparser->sLen - 1])\n"
        ,ns,ns
    );
    g->line2 += 8;

    fprintf(out,
        "#define YYCHECK_PUSH_TOKEN() \\\n"
        YYTAB "if(%sparser->sp - %sparser->pstack >= %sparser->pSize){\\\n"
        YYTAB YYTAB "size_t offset = %sparser->sp - %sparser->pstack;\\\n"
        YYTAB YYTAB "%sparser->pSize *= 2;\\\n"
        YYTAB YYTAB "%sparser->pstack = (%s *)%sparser->rtor(%sparser->pstack,sizeof(%s) * %sparser->pSize);\\\n"
        YYTAB YYTAB "%sparser->sp = %sparser->pstack + offset;\\\n"
        YYTAB "}\n"
        ,ns,ns,ns
        ,ns,ns
        ,ns
        ,ns,stype,ns,ns,stype,ns
        ,ns,ns
    );
    g->line2 += 7;

    yGenShiftTable(g,out);
    yGenGotoTable(g,out);
    yGenLhsTable(g,out);
    //yGenRuleLengthTable(g,out);
    //yGenRuleValueLengthTable(g,out);
    yGenSymbolTable(g,out);
    
    yGenReduce(g,out);


    fprintf(out,
        "int %sParser_init(%sParser *%sparser,%salloc altor,%srealloc rtor,%sfree dtor){\n"
        YYTAB "%sparser->altor = altor;\n"
        YYTAB "%sparser->dtor = dtor;\n"
        YYTAB "%sparser->rtor = rtor;\n"
        YYTAB "%sparser->sLen = 1;\n"
        YYTAB "%sparser->sSize = %sparser->pSize = 16;\n"
        YYTAB "%sparser->state = (int *)altor(sizeof(int) * %sparser->sSize);\n"
        YYTAB "%sparser->state[0] = 0;\n"
        YYTAB "%sparser->sp = %sparser->pstack = (%s *)altor(sizeof(%s) * %sparser->pSize);\n"
        YYTAB "return 0;\n"
        "}\n"
        ,ns,ns,ns,ns,ns,ns
        ,ns
        ,ns
        ,ns
        ,ns
        ,ns,ns
        ,ns,ns
        ,ns
        ,ns,ns,stype,stype,ns
    );
    g->line2 += 10;

    fprintf(out,
        "int %sParser_free(%sParser *%sparser){\n"
        YYTAB "%sparser->dtor(%sparser->state);\n"
        YYTAB "%sparser->dtor(%sparser->pstack);\n"
        YYTAB "return 0;\n"
        "}\n"
        ,ns,ns,ns
        ,ns,ns
        ,ns,ns
    );
    g->line2 += 5;

    fprintf(out,
        "int %sParser_acceptToken(%sParser *%sparser,int %stokenid){\n"
        YYTAB "int shifted = 0;\n"
        YYTAB "while(!shifted){\n"
        YYTAB YYTAB "int %saction = %sshift[YYSTATE() * %stokenCount + %stokenid];\n"
        YYTAB YYTAB "if(%saction > 0){\n"
        YYTAB YYTAB YYTAB "YYCHECK_PUSH_TOKEN();\n"
        YYTAB YYTAB YYTAB "*%sparser->sp++ = %sparser->token;\n"
        YYTAB YYTAB YYTAB "YYPUSH_STATE(%sparser,%saction - 1);\n"
        YYTAB YYTAB YYTAB "shifted = 1;\n"
        YYTAB YYTAB "}\n"
        YYTAB YYTAB "else if(%saction < 0){\n"
        YYTAB YYTAB YYTAB "%sParser_reduce(%sparser,-1 - %saction);\n"
        YYTAB YYTAB "}\n"
        YYTAB YYTAB "else {\n"
        YYTAB YYTAB YYTAB "%sparser->error = 1;\n"
        YYTAB YYTAB YYTAB "%sparser->errToken = %stokenid;\n"
        YYTAB YYTAB YYTAB "return -1;\n"
        YYTAB YYTAB "}\n"
        YYTAB "}\n"
        YYTAB "return 0;\n"
        "}\n"
        ,ns,ns,ns,ns
        ,ns,ns,ns,ns
        ,ns
        ,ns,ns
        ,ns,ns
        ,ns
        ,ns,ns,ns
        ,ns
        ,ns,ns
    );
    g->line2 += 18;

    fprintf(out,
        "int %sParser_printError(%sParser *%sparser,FILE *out){\n"
        YYTAB "if(%sparser->error){\n"
        YYTAB YYTAB "int index = YYSTATE() * %stokenCount;\n"
        YYTAB YYTAB "fprintf(out,\"unexpected token '%cs',was expecting one of:\\n\",%stokenNames[%sparser->errToken]);\n"
        YYTAB YYTAB "int i;\n"
        YYTAB YYTAB "for(i = 0;i < %stokenCount;i++){\n"
        YYTAB YYTAB YYTAB "if(%sshift[index + i] != 0){\n"
        YYTAB YYTAB YYTAB YYTAB "fprintf(out,\"    '%cs' ...\\n\",%stokenNames[i]);\n"
        YYTAB YYTAB YYTAB "}\n"
        YYTAB YYTAB "}\n"
        YYTAB "}\n"
        YYTAB "return 0;\n"
        "}\n"
        ,ns,ns,ns
        ,ns
        ,ns
        ,'%',ns,ns
        ,ns
        ,ns
        ,'%',ns
    );
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