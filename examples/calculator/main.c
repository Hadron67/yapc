#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include "parser.h"

double _Complex eval(const char *s,int *err){
    int tokenid;
    const char *sp = s;
    
    //this is the parser object
    yyParser parser;
    
    //initialize the parser
    yyParser_init(&parser);
    
    //since in yapc,the tokenizer calls the parser,
    //we should first scan the token from the input.
yylex:
    //in this example i just write the tokenizer by hand,
    //in fact in pactice we could use lex or re2c to generate it.
    
    while(*sp == ' ' || *sp == '\n' || *sp == '\t'){
        sp++;
    }
    if(!*sp){
        tokenid = T_EOF;
        goto yyparse;
    }
    switch(*sp){
        case '+':
            sp++;
            tokenid = T_PLUS;
            goto yyparse;
        case '-':
            sp++;
            tokenid = T_MINUS;
            goto yyparse;
        case '*':
            sp++;
            if(*sp == '*'){
                sp++;
                tokenid = T_EXP;
                goto yyparse;
            }
            else {
                tokenid = T_MULTIPLY;
                goto yyparse;                
            }
        case '/':
            sp++;
            tokenid = T_DIVIDE;
            goto yyparse;
        case '(':
            sp++;
            tokenid = T_BRA;
            goto yyparse;
        case ')':
            sp++;
            tokenid = T_KET;
            goto yyparse;
        case 's':
            sp++;
            if(*sp == 'i'){
                sp++;
                if(*sp == 'n'){
                    sp++;
                    if(*sp == 'h'){
                        sp++;
                        tokenid = T_SINH;
                        goto yyparse;
                    }
                    else{
                        tokenid = T_SIN;
                        goto yyparse;
                    }
                }
            }
            else if(*sp == 'q'){
                sp++;
                if(*sp == 'r'){
                    sp++;
                    if(*sp == 't'){
                        sp++;
                        tokenid = T_SQRT;
                        goto yyparse;
                    }
                }
            }
            goto yyunexpected;
        case 'c':
            sp++;
            if(*sp == 'o'){
                sp++;
                if(*sp == 's'){
                    sp++;
                    if(*sp == 'h'){
                        sp++;
                        tokenid = T_COSH;
                        goto yyparse;
                    }
                    else{
                        tokenid = T_COS;
                        goto yyparse;
                    }
                }
            }
            goto yyunexpected;
        case 't':
            sp++;
            if(*sp == 'a'){
                sp++;
                if(*sp == 'n'){
                    sp++;
                    if(*sp == 'h'){
                        sp++;
                        tokenid = T_TANH;
                        goto yyparse;
                    }
                    else{
                        tokenid = T_TAN;
                        goto yyparse;
                    }
                }
            }
            goto yyunexpected;
        case 'l':
            sp++;
            if(*sp == 'n'){
                sp++;
                tokenid = T_LN;
                goto yyparse;
            }
            goto yyunexpected;
        case 'e':   
            sp++;
            if(*sp == 'x'){
                sp++;
                if(*sp == 'p'){
                    sp++;
                    tokenid = T_CEXP;
                    goto yyparse;
                }
            }
            else{
                tokenid = T_E;
                goto yyparse;
            }
        case 'p':
            sp++;
            if(*sp == 'i'){
                sp++;
                tokenid = T_PI;
                goto yyparse;
            }
            goto yyunexpected;
        case 'I':
            sp++;
            tokenid = T_I;
            goto yyparse;
        case 'a':
            sp++;
            if(*sp == 's'){
                sp++;
                if(*sp == 'i'){
                    sp++;
                    if(*sp == 'n'){
                        sp++;
                        tokenid = T_ASIN;
                        goto yyparse;
                    }
                }
            }
            else if(*sp == 'c'){
                sp++;
                if(*sp == 'o'){
                    sp++;
                    if(*sp == 's'){
                        sp++;
                        tokenid = T_ACOS;
                        goto yyparse;
                    }
                }
            }
            else if(*sp == 't'){
                sp++;
                if(*sp == 'a'){
                    sp++;
                    if(*sp == 'n'){
                        sp++;
                        tokenid = T_ATAN;
                        goto yyparse;
                    }
                }
            }
            goto yyunexpected;
        default:
            if(*sp >= '0' && *sp <= '9'){
                double _Complex num = 0;
                while(*sp >= '0' && *sp <= '9'){
                    num *= 10;
                    num += *sp - '0';
                    sp++;
                }
                if(*sp == '.'){
                    sp++;
                    double i = 10;
                    while(*sp >= '0' && *sp <= '9'){
                        num += (*sp - '0') / i;
                        i *= 10;
                        sp++;
                    }
                }
                tokenid = T_NUM;
                parser.token = num;
                goto yyparse;
            }
            else {
                goto yyunexpected;
            }
    }
    
yyparse:
    //pass the token to the parser to parse.
    if(yyParser_acceptToken(&parser,tokenid)){
        //error occurs,print error message
        yyParser_printError(&parser,stderr);
        goto yyerr;
    }
    //if input is accepted
    if(!parser.done){
        goto yylex;
    }
    *err = 0;
    double _Complex ret = *parser.pstack;
    //destroy the parser.
    yyParser_free(&parser);
    return ret;
yyunexpected:
    fprintf(stderr,"unexpected character '%c'\n",*sp);
yyerr:
    yyParser_free(&parser);
    *err = 1;
    return 0;
}

int main(int agv,const char *ags[]){
    int err;
    if(agv != 2){
        fprintf(stderr,"exactly one argument is required.\n");
        return -1;
    }
    double _Complex z = eval(ags[1],&err);
    if(!err){
        printf("%lf + %lf * I\n",creal(z),cimag(z));
        return 0;
    }
    else {
        return -1;
    }
}
