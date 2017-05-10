#include <stdio.h>
#include <assert.h>

#include "scanner.h"
#include "parser.h"
#include "compile.h"
#include "execute.h"

int runLex(){
    FILE *in = fopen("test.p","ro");
    FILE *out = fopen("test.dyd","wo");
    FILE *err = stderr;

    yyScanner s;
    yyScanner_init(&s,in,err);
    
    yynode t;
    yyScanner_next(&s,&t);
    while(t.id != T_EOF){
        int i;
        for(i = 0;i < t.lineCount;i++){
            fprintf(out,"%16s %-2d\n","EOL",24); 
        }
        fprintf(out,"%16s %-2d \n",tokenNames[t.id - 1],t.id);
           
        yyScanner_next(&s,&t);
    };
    fprintf(out,"%16s %2d\n","EOF",25); 
    yyScanner_free(&s,NULL);
    fclose(in);
    fclose(out);
    return 0;
}

int runParsing(){
    FILE *in = fopen("test.p","ro");
    FILE *out = fopen("test.tree","wo");
    FILE *err = stderr;
    int status = 0;
    yyScanner s;
    yyScanner_init(&s,in,err);

    yyParser parser;
    yyParser_init(&parser);

    yyCompiler compiler;
    yyCompiler_init(&compiler,err,&s.pool);

    parser.userData = &compiler;

    while(1){
        yynode *t = &parser.token;
        yylex:
        yyScanner_next(&s,t);
        /* fall through */
        yyparse:
        if(compiler.status){
            goto yyexit;
        }
        if(compiler.halted){
            goto yyrun;
        }
        switch(yyParser_acceptToken(&parser,t->id)){
            case YY_SHIFT: goto yylex;
            case YY_REDUCE: goto yyparse;
            case YY_ACCEPT: goto yyaccept;
            case YY_ERR:
                yyParser_printError(&parser,err);
                fprintf(err," at line %d\n",t->line);
                goto yyexit;
        }
        assert(0);
    }

    yyaccept:
    yyParser_printCst(&parser,out);
    yoparray_dump(compiler.oparray,compiler.opLen,stdout);
    printf("\n\n");
    yvm vm;
    yyrun:
    yvm_init(&vm,stdin,stdout);
    yvm_loadProg(&vm,compiler.oparray);
    yvm_execute(&vm);
    yvm_free(&vm);
    /* fall through */
    yyexit:
    yyScanner_free(&s,NULL);
    yyParser_free(&parser);
    yyCompiler_free(&compiler);

    fclose(in);
    fclose(out);
    return 0;
}

int main(int agv,const char *ags[]){
    //runLex();
    runParsing();
    return 0;
}
