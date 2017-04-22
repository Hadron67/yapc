#include <stdio.h>
#include <stdlib.h>
#include "parser.h"



int main(int agv,const char *ags[]){
    yyParser parser;
    yyParser_init(&parser,malloc,realloc,free);
    
    parser.token = 1;
    yyParser_acceptToken(&parser,T_NUM);
    yyParser_acceptToken(&parser,T_PLUS);
    parser.token = 2;
    yyParser_acceptToken(&parser,T_NUM);
    yyParser_acceptToken(&parser,T_NUM);
    yyParser_printError(&parser,stdout);
    
    yyParser_free(&parser);
}
