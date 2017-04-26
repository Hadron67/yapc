#include <stdio.h>

#include "scanner.h"

int main(int agv,const char *ags[]){
    FILE *in = fopen("test.p","ro");
    FILE *out = fopen("test.dyd","wo");
    FILE *err = stderr;

    yyScanner s;
    yyScanner_init(&s,in,err);
    
    yyToken t;
    do{
        yyScanner_next(&s,&t);
        
        fprintf(out,"%s %d\n",tokenNames[t.id - 1],t.id); 
           
    }while(t.id != T_EOF);
    yyScanner_free(&s);
    fclose(in);
    fclose(out);
    return 0;
}
