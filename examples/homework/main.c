#include <stdio.h>

#include "scanner.h"
#include "parser.h"

int runLex(){
    FILE *in = fopen("test.p","ro");
    FILE *out = fopen("test.dyd","wo");
    FILE *err = stderr;

    yyScanner s;
    yyScanner_init(&s,in,err);
    
    yyToken t;
    yyScanner_next(&s,&t);
    while(t.id != T_EOF){
        int i;
        for(i = 0;i < t.lineCount;i++){
            fprintf(out,"%16s %-2d\n","EOL",24); 
        }
        if(t.id == T_ID){
            fprintf(out,"%16s %-2d (%s)\n",tokenNames[t.id - 1],t.id,t.u.image.s); 
        }
        else{
            fprintf(out,"%16s %-2d \n",tokenNames[t.id - 1],t.id);
        }
           
        yyScanner_next(&s,&t);
    };
    fprintf(out,"%16s %2d\n","EOF",25); 
    yyScanner_free(&s);
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

    yyToken t;
    while(!parser.done){
        yyScanner_next(&s,&t);
        if(yyParser_acceptToken(&parser,t.id) != YY_OK){
            yyParser_printError(&parser,err);
            fprintf(err," at line %d\n",t.line);
            status = -1;
            break;
        }
    }

    if(!status){
        printf("accepted!\n");
        yyParser_printCst(&parser,out);
    }

    yyScanner_free(&s);
    yyParser_free(&parser);
    fclose(in);
    fclose(out);
    return 0;
}

int main(int agv,const char *ags[]){
    runLex();
    runParsing();
    return 0;
}
